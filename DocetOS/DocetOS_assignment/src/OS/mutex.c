#include "OS/mutex.h"

void OS_mutex_aquire(OS_mutex_t * mutex, OS_TCB_t * current_tcb) {
	// In this implementation STREX is not always called
	// Unsure if this might cause problems or not.
	
	uint32_t repeat = 0;
	
	do {
		uint32_t notification_counter = getNotificationCounter();
		
		//Equivalent to: OS_TCB_t * tcb = mutex->tcb
		OS_TCB_t * mutex_tcb = (OS_TCB_t *) __LDREXW((uint32_t *)&(mutex->tcb));
		
		if (mutex_tcb == 0) {
			// Equivalent to: mutex->tcb = current_tcb
			repeat = __STREXW ((uint32_t) current_tcb, (uint32_t *)&(mutex->tcb));
			//(void) notification_counter;
		}
		else if (mutex_tcb != current_tcb) {
			int32_t *mutex_TCBPriority = 0;
			do {
				// Equivalent to: int32_t *mutex_TCBPriority = mutex_tcb->priority
				mutex_TCBPriority = (int32_t *) __LDREXW((uint32_t *)&(mutex_tcb->priority));
				current_tcb->tempPriority = (int32_t) mutex_tcb->priority;
			
			} while(__STREXW ((uint32_t) mutex_TCBPriority, (uint32_t *)&(current_tcb->tempPriority)));
				
			OS_wait(notification_counter);
			repeat = 1;
		}
	} while (repeat);
	
	mutex->counter++;
}

void OS_mutex_acquire_refactor(OS_mutex_t * mutex, OS_TCB_t * current_tcb) {
	// In this implementation STREX is always called unless the 
	// break statement is hit... I think
	
	do {
		uint32_t notification_counter = getNotificationCounter();
		OS_TCB_t * mutex_tcb = (OS_TCB_t *) __LDREXW((uint32_t *)&(mutex->tcb));
		
		if (!mutex_tcb) {
			if (mutex_tcb == current_tcb) {
				break;
			}
			else {
				OS_wait(notification_counter);
				continue;
			}
		}
		
	} while (__STREXW ((uint32_t) current_tcb, (uint32_t *)&(mutex->tcb)));
	
	mutex->counter++;
}


void OS_mutex_release(OS_mutex_t * mutex, OS_TCB_t * current_tcb) {
	// Safety check:
	if (current_tcb == mutex->tcb) {
		// Decrement counter field
		mutex->counter--;
		// If it has reached zero:
		if (mutex->counter == 0) {
			// Set the TCB field to zero too
			mutex->tcb = 0;
			
			if (current_tcb->tempPriority != -1) {
				current_tcb->tempPriority = -1;
				// Now need a new wait_delegate to change the task priority
			}
			// Call OS_notifyAll()
			OS_notifyAll();
		}
	}
}