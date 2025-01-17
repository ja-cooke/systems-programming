#include "OS/sleep.h"
#include "OS/os.h"
#include "OS/scheduler.h"

void OS_sleep(uint32_t sleepTime){
	uint32_t sleep_counter = getSleepCounter();
	uint32_t currentTime = OS_elapsedTicks();
	OS_TCB_t * currentTCB = OS_currentTCB();
	
	// Store the next wake time
	currentTCB->data = currentTime + sleepTime;
	
	// Set the status to sleep
	currentTCB->state |= TASK_STATE_SLEEP; // Need to check that this works
	
	// Switch task
	OS_sleepHeap(sleep_counter);
}