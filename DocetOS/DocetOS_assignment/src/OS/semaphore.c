#include "OS/semaphore.h"

void OS_semaphore_acquire(OS_semaphore_t *semaphore) {
	// In this implementation STREX is not always called
	// Unsure if this might cause problems or not.
	
	uint32_t accessError = 0;
	
	do {
		uint32_t notification_counter = getNotificationCounter();
		
		/* Protect semaphores against concurrent access: */
		
		//Equivalent to: OS_semaphore_t *semaphoresAvailable = semaphore->available
		uint32_t semaphoresAvailable = (uint32_t) __LDREXW((uint32_t *)&(semaphore->available));
		
		if (semaphoresAvailable > 0) {
			// Equivalent to: semaphore->available = --semaphoresAvailable
			accessError = __STREXW ((uint32_t) --semaphoresAvailable, (uint32_t *)&(semaphore->available));
		}
		else {
			OS_wait(notification_counter);
			accessError = 1;
		}
	} while(accessError);
}

void OS_semaphore_release(OS_semaphore_t *semaphore) {
		
	uint32_t semaphoresAvailable = 0;
	
	do {
		/* Protect semaphores against concurrent access: */
		
		//Equivalent to: semaphoresAvailable = semaphore->available
		semaphoresAvailable = (uint32_t) __LDREXW((uint32_t *)&(semaphore->available));
		
		// Equivalent to: semaphore->available = semaphoresAvailable++
	} while (__STREXW ((uint32_t) ++semaphoresAvailable, (uint32_t *)&(semaphore->available)));
			
	// Would be a good idea to write in seperate notification for semaphores and mutexes
	OS_notifyAll();
}

/* 
 * Same process flow as for regular semaphores, but with boolean true-false
 * logic instead of a count.
 */
void OS_semBinary_acquire(OS_semBinary_t *semaphore) {

	uint32_t accessError = 0;
	
	do {
		uint32_t notification_counter = getNotificationCounter();
		
		/* Protect semaphores against concurrent access: */
		
		//Equivalent to: OS_semaphore_t *semaphoresAvailable = mutex->tcb
		uint32_t semaphoreAvailable = (uint32_t) __LDREXW((uint32_t *)&(semaphore->available));
		
		if (semaphoreAvailable) {
			// Equivalent to: semaphore->available = 0
			accessError = __STREXW ((uint32_t) 0, (uint32_t *)&(semaphore->available));
		}
		else {
			OS_wait(notification_counter);
			accessError = 1;
		}
	} while (accessError);
}

void OS_semBinary_release(OS_semBinary_t *semaphore) {
	uint32_t semaphoreAvailable = 0;
	
	do {
		/* Protect semaphores against concurrent access: */
		
		//Equivalent to: semaphoreAvailable = semaphore->available
		semaphoreAvailable = (uint32_t) __LDREXW((uint32_t *)&(semaphore->available));
		
		// Equivalent to: semaphore->available = semaphoreAvailable++
	} while (__STREXW ((uint32_t) ++semaphoreAvailable, (uint32_t *)&(semaphore->available)));
			
	// Would be a good idea to write in seperate notification for semaphores and mutexes
	OS_notifyAll();
}