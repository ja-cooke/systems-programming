#include "OS/mutex.h"

/* 
 * RE-ENTRANT MUTEX
 * 
 * For fixed priority tasks. A seperate mutex must be used for each priority 
 * level in order to prevent deadlocks.
 */

/* 
 * Can be called multiple times by the TCB already holding the mutex,
 * a count of the number of times is kept.
 * Once it has been released the same number of times it is assumed that
 * all of the resources are available again, and the TCB is removed from the 
 * mutex TCB field.
 */
void OS_mutex_acquire(OS_mutex_t * mutex) {
	OS_TCB_t * current_tcb = OS_currentTCB();
	
	uint32_t accessError = 0;
	
	do {
		uint32_t notification_counter = getNotificationCounter();
		
		//Equivalent to: OS_TCB_t * tcb = mutex->tcb
		OS_TCB_t * mutex_tcb = (OS_TCB_t *) __LDREXW((uint32_t *)&(mutex->tcb));
		
		if (mutex_tcb == 0) {
			// Equivalent to: mutex->tcb = current_tcb
			accessError = __STREXW ((uint32_t) current_tcb, (uint32_t *)&(mutex->tcb));
		}
		else if (mutex_tcb != current_tcb) {
			OS_wait(notification_counter);
			accessError = 1;
		}
	} while (accessError);
	
	mutex->counter++;
}

/* 
 * A task can call to release the mutex multiple times
 * only when it has released as many times as it has called
 * to acquire will it actually be released.
 */
void OS_mutex_release(OS_mutex_t * mutex) {
	 OS_TCB_t * current_tcb = OS_currentTCB();
	
	// Safety check:
	if (current_tcb == mutex->tcb) {
		mutex->counter--;
		if (mutex->counter == 0) {
			/* TCB is removed from the mutex, effectively releasing it */
			mutex->tcb = 0;
			
			OS_notifyAll();
		}
	}
}