#include "OS/mutex.h"

void OS_mutex_aquire(OS_mutex_t * mutex, OS_TCB_t * current_tcb) {
	uint32_t repeat = 0;
	
	do {
		//Equivalent to: OS_TCB_t * tcb = mutex->tcb
		OS_TCB_t * mutex_tcb = (OS_TCB_t *) __LDREXW((uint32_t *)&(mutex->tcb));
		
		if (mutex_tcb == 0) {
			// Equivalent to: mutex->tcb = current_tcb
			repeat = __STREXW ((uint32_t) current_tcb, (uint32_t *)&(mutex->tcb));
		}
		else if (mutex_tcb != current_tcb) {
			OS_wait();
			repeat = 1;
		}
	} while (repeat);
	
	mutex->counter++;
}

void OS_mutex_release(OS_mutex_t * mutex) {

}