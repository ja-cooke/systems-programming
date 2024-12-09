#include "sleep.h"
#include "OS/os.h"

void OS_sleep(uint32_t sleep_time){
	uint32_t current_time = OS_elapsedTicks();
	OS_TCB_t * current_TCB = OS_currentTCB();
	
	// Store the next wake time
	current_TCB->data = current_time + sleep_time;
	
	// Set the status to sleep
	current_TCB->state |= TASK_STATE_SLEEP; // Need to check that this works
	
	// Switch task
	OS_yield();
	
}