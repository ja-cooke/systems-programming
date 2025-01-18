#include "OS/sleep.h"
#include "OS/os.h"
#include "OS/scheduler.h"

void OS_sleep(uint32_t sleepTime){
	uint32_t sleep_counter = getSleepCounter();
	uint32_t currentTime = OS_elapsedTicks();
	OS_TCB_t * currentTCB = OS_currentTCB();
	
	uint32_t wakeTime = currentTime + sleepTime;
	
	/* Check for overflows */
	if (wakeTime > currentTime) {
		// Store the next wake time
		currentTCB->data = currentTime + sleepTime;
	}
	else {
		/* If the wake time has overflowed, the time is set to the last time before
		 * SysTick overflows. This ensures that sleeping tasks are never risen to the
		 * top of the sleepHeap prematurely which would cause sleeping tasks to wake 
		 * at the next SysTick after they are put to sleep.
		 * This implementation means that all tasks will wake when SysTick overflows
		 * however this full sleep reset design is safe from tasks being put
		 * permanently to sleep due to overflows.
		 */
		currentTCB->data = (uint32_t)(-1L);
	}
	
	
	
	// Set the status to sleep
	currentTCB->state |= TASK_STATE_SLEEP; // Need to check that this works
	
	// Switch task
	OS_sleepHeap(sleep_counter);
}