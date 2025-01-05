#include "OS/semaphore.h"

void OS_semaphore_acquire(OS_semaphore_t *semaphore) {
	// In this implementation STREX is not always called
	// Unsure if this might cause problems or not.
	
	uint32_t repeat = 0;
	
	// TODO
	
	do {
		uint32_t notification_counter = getNotificationCounter();
		
		/* Protect semaphores against concurrent access: */
		
		//Equivalent to: OS_semaphore_t *semaphoresAvailable = mutex->tcb
		OS_semaphore_t *semaphoresAvailable = (OS_semaphore_t *) __LDREXW((uint32_t *)&(semaphore->available));
		
		if (semaphoresAvailable > 0) {
			// Equivalent to: semaphore->available = semaphoresAvailable--
			repeat = __STREXW ((uint32_t) semaphoresAvailable--, (uint32_t *)&(semaphore->available));
		}
		else if (semaphoresAvailable == 0) {
			OS_wait(notification_counter);
			repeat = 1;
		}
	} while (repeat);
}

void OS_semaphore_acquire_refactor(OS_semaphore_t * mutex) {
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
	
	//mutex->counter++;
}


void OS_semaphore_release(OS_semaphore_t *semaphore) {
		
	uint32_t semaphoresAvailable = 0;
	
	do {
		/* Protect semaphores against concurrent access: */
		
		//Equivalent to: semaphoresAvailable = semaphore->available
		semaphoresAvailable = (uint32_t) __LDREXW((uint32_t *)&(semaphore->available));
		
		// Equivalent to: semaphore->available = semaphoresAvailable++
	} while (__STREXW ((uint32_t) semaphoresAvailable++, (uint32_t *)&(semaphore->available)));
			
	// Would be a good idea to write in seperate notification for semaphores and mutexes
	OS_notifyAll();
}