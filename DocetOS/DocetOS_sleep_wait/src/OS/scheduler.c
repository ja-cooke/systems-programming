#define OS_INTERNAL

#include "OS/scheduler.h"
#include "OS/os.h"

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

static _OS_tasklist_t task_list = {.head = 0};
static _OS_tasklist_t wait_list = {.head = 0};
static _OS_tasklist_t pending_list = {.head = 0};

static void list_add(_OS_tasklist_t *list, OS_TCB_t *task) {
	if (!(list->head)) {
		task->next = task;
		task->prev = task;
		list->head = task;
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

static OS_TCB_t * list_pop_sl (_OS_tasklist_t *list) {
	OS_TCB_t *task;
	task = list->head;
	list->head = task->next;
	/* I do not believe that this implementation of LDREXW/STREXW is required
	 * The first line does not modify the list as is the case in the push function
	 * it only reads it, the modification occurs in a single instruction which cannot
	 * (to my knowledge) be interrupted. */
	/* do {
		// task = list->head
		task = (OS_TCB_t *) __LDREXW ((uint32_t *)&(list->head));
	} while (__STREXW ((uint32_t) task->next, (uint32_t *)&list->head)); // list->head = task->next */
	return task;
}

/* -------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */

/* Round-robin scheduler */
OS_TCB_t const * _OS_schedule(void) {
	
	
	/* Add all notified tasks back into the task list */
	while (pending_list.head) {
		OS_TCB_t * tcb;
		tcb = list_pop_sl(&pending_list);
		list_add(&task_list, tcb);
	}
	
	if (task_list.head) {
		OS_TCB_t * first_task = task_list.head;
		
		/* Cycle through all tasks until finding one that is awake */
		do {
			task_list.head = task_list.head->next;
				
			// Check the sleep state, do not return if true
			if (task_list.head->state &= TASK_STATE_SLEEP){
				uint32_t wake_time;
				uint32_t current_time;
				
				wake_time = task_list.head->data;
				current_time = OS_elapsedTicks(); // what to do after overflow?
				
				// If the wake time has passed
				if (current_time > wake_time) {
					// Clear the sleep flag
					task_list.head->state &= ~TASK_STATE_SLEEP;
					task_list.head->state &= ~TASK_STATE_YIELD;
					return task_list.head;
				}
			}
			else {
				task_list.head->state &= ~TASK_STATE_YIELD;
				return task_list.head;
			}
		} while (task_list.head != first_task);
		// Return idle if all tasks are alseep
		return _OS_idleTCB_p;
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

/* 'Add task' */
void OS_addTask(OS_TCB_t * const tcb) {
	list_add(&task_list, tcb);
}

/* SVC handler that's called by _OS_task_end when a task finishes.  Removes the
   task from the scheduler and then queues PendSV to reschedule. */
void _OS_taskExit_delegate(void) {
	// Remove the given TCB from the list of tasks so it won't be run again
	OS_TCB_t * tcb = OS_currentTCB();
	list_remove(&task_list, tcb);
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

void _OS_wait_delegate(void) {
	OS_TCB_t * tcb = task_list.head;
	list_remove(&task_list, tcb);
	list_push_sl(&wait_list, tcb);
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

void OS_notifyAll(void) {
	while (wait_list.head) {
		OS_TCB_t * tcb;
		tcb = list_pop_sl(&wait_list);
		list_push_sl(&pending_list, tcb);
	}
}
