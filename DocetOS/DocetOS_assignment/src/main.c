#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "OS/os.h"
#include "tasks.h"

/******************************************************************************/
/************************** DOCET OS DEMONSTRATION ****************************/
/******************************************************************************/

/*
 * New features added:
 * - Re-Entrant Mutex
 * - Static Memory Allocation
 * - Dynamic Memory Allocation
 * - Fixed Priority Scheduler
 * - Binary and Counting Semaphores
 * - Efficient Sleeping using a Generic Binary Heap
 * - Inter-Task Communication using a Pointer Queue
 */

/* MAIN FUNCTION */

/*
 * Demonstration task code can be found in tasks.c
 *
 */

int main(void) {
	/* Initialises the microcontroller */
	configClock();
	configUSART2(38400);
	
	/* Reserves memory for data structures */
	initialiseOSResources();
	
	/* Reserve memory for stacks and TCBs.
	 * Remember that stacks must be 8-byte aligned. 
	 *
	 * Attempting the change the stacks and TCBs that are initialised
	 * may result in hard faults for reasons undetermined. However, 
	 * they do not need to be added to the OS. Hence TCB2 is initialised
	 * but not used.
	 */
	static uint32_t stack1[128] __attribute__ (( aligned(8) ));
	static uint32_t stack3[128] __attribute__ (( aligned(8) ));
	static uint32_t stack4[128] __attribute__ (( aligned(8) ));
	static uint32_t stack5[128] __attribute__ (( aligned(8) ));
	static uint32_t stack6[128] __attribute__ (( aligned(8) ));
	
	static uint32_t stack2[128] __attribute__ (( aligned(8) ));
	
	static OS_TCB_t TCB1 = {.priority = 0};
	static OS_TCB_t TCB3 = {.priority = 2};
	static OS_TCB_t TCB4 = {.priority = 3};
	static OS_TCB_t TCB5 = {.priority = 3};
	static OS_TCB_t TCB6 = {.priority = 4};
	
	static OS_TCB_t TCB2 = {.priority = 0};

	/* Initialise the TCBs using the two functions above */
	OS_initialiseTCB(&TCB1, stack1+128, veryHighPriorityTask, NULL);
	OS_initialiseTCB(&TCB3, stack3+128, greedyTask, NULL);
	OS_initialiseTCB(&TCB4, stack4+128, chattyTask, NULL);
	OS_initialiseTCB(&TCB5, stack5+128, listeningTask, NULL);
	OS_initialiseTCB(&TCB6, stack6+128, lowPriorityTask, NULL);
	
	OS_initialiseTCB(&TCB2, stack2+128, sleepTest1, NULL);
	
	/* Add the tasks to the scheduler */
 	OS_addTask(&TCB1);
	OS_addTask(&TCB3);
	OS_addTask(&TCB4);
	OS_addTask(&TCB5);
	OS_addTask(&TCB6);
	
	/* Start the OS */
	OS_start();
	
}
