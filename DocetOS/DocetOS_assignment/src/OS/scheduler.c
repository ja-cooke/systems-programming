#define OS_INTERNAL

#include "OS/scheduler.h"
#include "OS/os.h"
#include "OS/static_alloc.h"
#include "OS/heap.h"

#include "stm32f4xx.h"

#include <string.h>

/* This is an implementation of an extremely simple round-robin scheduler.

   A task list structure is declared.  Tasks are added to the list to create a circular buffer.
	 When tasks finish, they are removed from the list.  When the scheduler is invoked, it simply
	 advances the head pointer, and returns it.  If the head pointer is null, a pointer to the
	 idle task is returned instead.
	 
	 The scheduler is reasonably efficient but not very flexible.  The "yield" flag is not
	 checked, but merely cleared before a task is returned, so OS_yield() is equivalent to
	 OS_schedule() in this implementation.
*/

static _OS_fpSchedule_t schedule = {.priorityArray = 0};
static _OS_tasklist_t wait_list = {.head = 0};
static _OS_tasklist_t pending_list = {.head = 0};

static void *store[20] = {0};
static int32_t comparator(void *input_a, void *input_b);
static OS_heap_t sleepHeap = HEAP_INITIALISER(store, comparator, OS_BINARY_SEMAPHORE_STATIC_INITIALISER);

// Used by OS_wait to ensure that no notifications have occurred whilst it is already running
static uint32_t notification_counter = 0;
// Used by OS_sleepHeap to ensure that no notifications have occurred whilst it is already running
static uint32_t sleep_counter = 0;

static void list_add(_OS_tasklist_t *list, OS_TCB_t *task) {
	/* If the list head is null  point the list to the task*/
	if (!(list->head)) {
		task->next = task;
		task->prev = task;
		list->head = task;
	/* Otherwise, make the task the new head of the list */
	} else {
		task->next = list->head;
		task->prev = list->head->prev;
		task->prev->next = task;
		list->head->prev = task;
	}
}

static void list_remove(_OS_tasklist_t *list, OS_TCB_t *task) {
	// – Check to see if the item being removed is the only one in the list. If it is,
	// you’ll need to set the head pointer to zero.
	if(task->next == task){
		list->head = 0;
	}
	//– Remove the item by updating the pointers items either side of it to point
	// to each other.
	else{
		OS_TCB_t *next;
		OS_TCB_t *prev;
		
		next = task->next;
		prev = task->prev;
		
		next->prev = prev;
		prev->next = next;
		//– Update the head pointer if the item being removed was the head of the
		//list.
		if(task == list->head){
			list->head = next;
		}
	}
}

static void list_push_sl(_OS_tasklist_t *list, OS_TCB_t *task) {
	do {
		// Equivalent to: new_task->next = list->head;
		OS_TCB_t *head = (OS_TCB_t *) __LDREXW ((uint32_t *)&(list->head));
		task->next = head;
		// Equivalent to: list->head = new_task;
	} while (__STREXW ((uint32_t) task, (uint32_t *)&(list->head)));
}

// TODO
// Currently, popping from the pending list always returns the tasks to the same priority or something
static OS_TCB_t * list_pop_sl (_OS_tasklist_t *list) {
	OS_TCB_t *task;
	do {
		// task = list->head
		task = (OS_TCB_t *) __LDREXW ((uint32_t *)&(list->head));
		if (!task) {
			__CLREX();
			return 0;
		}
	} while (__STREXW ((uint32_t) task->next, (uint32_t *)&list->head)); // list->head = task->next */
	return task;
}

/* -------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */

static OS_TCB_t * roundRobin(_OS_tasklist_t *task_list) {

	/* Returns the next task if the list is not empty. */ 
	if (task_list->head) {
		task_list->head = task_list->head->next;
		task_list->head->state &= ~TASK_STATE_YIELD;
		return task_list->head;
	}
	else {
		/* List Empty, no tasks to run. */
		return 0;
	}
}

static void OS_wake(void) {
	OS_semBinary_acquire(&sleepHeap.accessToken);
	
	/* is it possible to solve the overflow problem by implementing
		 * current_time and wake_time as signed integers */
	uint32_t currentTime = OS_elapsedTicks(); // what to do after overflow?
	
	/* Overflow solution:
	 * if timerOverflow is set to 1 all tasks will be woken up
	 */
	uint32_t timerOverflow = SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk;
	
	uint32_t wakeTime = ((OS_TCB_t *) sleepHeap.store)->priority;
	
	while(!heap_isEmpty(&sleepHeap)) {
		// If the wake time has passed
		if (currentTime > wakeTime || timerOverflow) {
			// Clear the sleep flag
			OS_TCB_t *tcb = heap_extract(&sleepHeap);
			tcb->state &= ~TASK_STATE_SLEEP;
			tcb->state &= ~TASK_STATE_YIELD;
			list_push_sl(&pending_list, tcb);
		}
		else{
			break;
		}
	}
	OS_semBinary_release(&sleepHeap.accessToken);
}

static void sortPending(void) {
	OS_TCB_t * tcb;
	while ((tcb = list_pop_sl(&pending_list))) {
		// DANGER ZONE
		list_add(schedule.priorityArray[tcb->priority], tcb);
		// DANGER ZONE
	}
}

/* Fixed Priority Array of Round-Robins scheduler */
OS_TCB_t const * _OS_schedule(void) {
	/* Tasks which have finished sleeping are added to the task lists */ 
	OS_wake();
	/* Adds all notified waiting tasks back into the task lists */
	sortPending();
	
	/* Checks priortyArray for tasks in each task list, highest priority first */
	for (uint32_t i = 0; i < maxPriorities; i++) {
		_OS_tasklist_t * task_list = schedule.priorityArray[i];
		
		OS_TCB_t * tcb;
		if((tcb = roundRobin(task_list))) {
			return tcb;
		}
	}
	
	// No tasks are runnable, so return the idle task
	return _OS_idleTCB_p;
}

/* -------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */


/* Initialises a task control block (TCB) and its associated stack.  See os.h for details. */
void OS_initialiseTCB(OS_TCB_t * TCB, uint32_t * const stack, void (* const func)(void const * const), void const * const data) {
	TCB->sp = stack - (sizeof(_OS_StackFrame_t) / sizeof(uint32_t));
	TCB->state = 0;
	TCB->prev = TCB->next = 0;
	_OS_StackFrame_t *sf = (_OS_StackFrame_t *)(TCB->sp);
	/* By placing the address of the task function in pc, and the address of _OS_task_end() in lr, the task
	   function will be executed on the first context switch, and if it ever exits, _OS_task_end() will be
	   called automatically */
	*sf = (_OS_StackFrame_t) {
		.r0 = (uint32_t)(data),
		.r1 = 0,
		.r2 = 0,
		.r3 = 0,
		.r4 = 0,
		.r5 = 0,
		.r6 = 0,
		.r7 = 0,
		.r8 = 0,
		.r9 = 0,
		.r10 = 0,
		.r11 = 0,
		.r12 = 0,
		.lr = (uint32_t)_OS_task_end,
		.pc = (uint32_t)(func),
		.psr = xPSR_T_Msk  /* Sets the thumb bit to avoid a big steaming fault */
	};
}

/* -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 */

/* 'Add task' 
 *
 * Redesign will include logic to build priority array if not already constructed
 */

void OS_constructSchedule(void) {
	/* Static allocation of enough memory to hold the schedule */
	_OS_tasklist_t * listAddresses = static_alloc(maxPriorities * sizeof(_OS_tasklist_t));
	
	for (uint32_t i = 0; i < maxPriorities; i++) {
		schedule.priorityArray[i] = &listAddresses[i];
		schedule.priorityArray[i]->priority = i;
	}
}

void OS_addTask(OS_TCB_t * const tcb) {
	
	/* If empty, construct the full scheduler priority array */
	if (*schedule.priorityArray == 0) {
		OS_constructSchedule();
	}
	
	/* Acquire the task_list of correct priority from the schedule */
	uint32_t tcbPriority = tcb->priority;
	_OS_tasklist_t * task_list_ptr = schedule.priorityArray[tcbPriority];
	
	list_add(task_list_ptr, tcb);
}

/* -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 */

/* SVC handler that's called by _OS_task_end when a task finishes.  Removes the
   task from the scheduler and then queues PendSV to reschedule. */
void _OS_taskExit_delegate(void) {
	// Remove the given TCB from the list of tasks so it won't be run again
	OS_TCB_t *tcb = OS_currentTCB();
	
	/* Acquire the task_list of correct priority from the schedule */
	// DANGER ZONE
	uint32_t tcbPriority = tcb->priority;
	_OS_tasklist_t * task_list_ptr = schedule.priorityArray[tcbPriority];
	// DANGER ZONE
	list_remove(task_list_ptr, tcb);
	
	/* Initiate context switch */
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

/* SVC handler that adds the current tcb to the wait list. Effectively sets
   the task to sleep until the OS is notified. */
void _OS_wait_delegate(_OS_SVC_StackFrame_t * const stack) {
	
	/* Checks the stack to see if the value of the notification
	 * counter has been changed before continuing. */
	if (stack->r0 != notification_counter) {
		// return 1 to indicate failure
		stack->r0 = 1;
	}
	else {
		OS_TCB_t * tcb = OS_currentTCB();
		
		/* Acquire the task_list of correct priority from the schedule */
		// DANGER ZONE
		uint32_t tcbPriority = tcb->priority;
		_OS_tasklist_t * task_list_ptr = schedule.priorityArray[tcbPriority];
		// DANGER ZONE
		
		/* Add the tcb to the wait list */
		list_remove(task_list_ptr, tcb);
		list_push_sl(&wait_list, tcb);
		
		// return 0 to indicate success
		stack->r0 = 0;
	}
	/* Set PendSV bit for context switch */
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

void _OS_sleepHeap_delegate(_OS_SVC_StackFrame_t * const stack) {
	/* Checks the stack to see if the value of the sleep
	 * counter has been changed before continuing. */
	if (stack->r0 != sleep_counter) {
		// return 1 to indicate failure
		stack->r0 = 1;
	}
	else {
		OS_TCB_t * tcb = OS_currentTCB();
		
		/* Acquire the task_list of correct priority from the schedule */
		// DANGER ZONE
		uint32_t tcbPriority = tcb->priority;
		_OS_tasklist_t * task_list_ptr = schedule.priorityArray[tcbPriority];
		// DANGER ZONE
		
		/* Add the tcb to the sleepHeap*/
		list_remove(task_list_ptr, tcb);
		
		// DANGER ZONE
		OS_semBinary_acquire(&sleepHeap.accessToken);
		heap_insert(&sleepHeap, tcb);
		OS_semBinary_release(&sleepHeap.accessToken);
		// DANGER ZONE
		//list_push_sl(&wait_list, tcb);
		
		// return 0 to indicate success
		stack->r0 = 0;
	}
	/* Set PendSV bit for context switch */
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;

}

void OS_notifyAll(void) {
	// check code for fast-fail OS_wait() function
	notification_counter++;
	
	while (wait_list.head) {
		OS_TCB_t * tcb;
		tcb = list_pop_sl(&wait_list);
		list_push_sl(&pending_list, tcb);
	}
}

uint32_t getNotificationCounter(void){
	return notification_counter;
}

uint32_t getSleepCounter(void){
	return sleep_counter;
}

static int32_t comparator(void *input_a, void *input_b){
	
	OS_TCB_t *input_a_cast = input_a;
	OS_TCB_t *input_b_cast = input_b;
	
	uint32_t priority_a = input_a_cast->priority;
	uint32_t priority_b = input_b_cast->priority;
	// Return > 0 if the first parameter is "greater"
	if(priority_a > priority_b){
		return 1;
	}
	// Return < 0 if the first parameter is "less"
	else if(priority_a < priority_b){
		return -1;
	}
	// Return 0 if the first parameter is "equal"
	else{
		return 0;
	}
}