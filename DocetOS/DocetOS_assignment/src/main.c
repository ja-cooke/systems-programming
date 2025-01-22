#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "OS/os.h"
#include "tasks.h"

/* MAIN FUNCTION */

int main(void) {
	/* Initialises the microcontroller */
	configClock();
	configUSART2(38400);
	
	/* Reserves memory for data structures */
	initialiseOSResources();
	
	/* Reserve memory for two stacks and two TCBs.
	   Remember that stacks must be 8-byte aligned. */
	static uint32_t stack1[128] __attribute__ (( aligned(8) ));
	static uint32_t stack2[128] __attribute__ (( aligned(8) ));
	static uint32_t stack3[128] __attribute__ (( aligned(8) ));
	static uint32_t stack4[128] __attribute__ (( aligned(8) ));
	static uint32_t stack5[128] __attribute__ (( aligned(8) ));
	static uint32_t stack6[128] __attribute__ (( aligned(8) ));
	static uint32_t stack7[128] __attribute__ (( aligned(8) ));
	static uint32_t stack8[128] __attribute__ (( aligned(8) ));
	static uint32_t stack9[128] __attribute__ (( aligned(8) ));
	static uint32_t stack10[128] __attribute__ (( aligned(8) ));
	
	static OS_TCB_t TCB1 = {.priority = 0};
	static OS_TCB_t TCB2 = {.priority = 0};
	static OS_TCB_t TCB3 = {.priority = 1};
	static OS_TCB_t TCB4 = {.priority = 0};
	static OS_TCB_t TCB5 = {.priority = 1};
	static OS_TCB_t TCB6 = {.priority = 0};
	static OS_TCB_t TCB7 = {.priority = 2};
	static OS_TCB_t TCB8 = {.priority = 3};
	static OS_TCB_t TCB9 = {.priority = 3};
	static OS_TCB_t TCB10 = {.priority = 4};

	/* Initialise the TCBs using the two functions above */
	OS_initialiseTCB(&TCB1, stack1+128, veryHighPriorityTask, NULL);
	OS_initialiseTCB(&TCB2, stack2+128, sleepTest2, NULL);
	OS_initialiseTCB(&TCB3, stack3+128, sleepTest1, NULL);
	OS_initialiseTCB(&TCB4, stack4+128, sleepTest2, NULL);
	OS_initialiseTCB(&TCB5, stack5+128, sleepTest1, NULL);
	OS_initialiseTCB(&TCB6, stack6+128, sleepTest2, NULL);
	OS_initialiseTCB(&TCB7, stack7+128, greedyTask, NULL);
	OS_initialiseTCB(&TCB8, stack8+128, chattyTask, NULL);
	OS_initialiseTCB(&TCB9, stack9+128, listeningTask, NULL);
	OS_initialiseTCB(&TCB10, stack10+128, lowPriorityTask, NULL);
	
	/* Add the tasks to the scheduler */
 	OS_addTask(&TCB1);
//	OS_addTask(&TCB2);
//	OS_addTask(&TCB3);
//	OS_addTask(&TCB4);
//	OS_addTask(&TCB5);
//	OS_addTask(&TCB6);
	OS_addTask(&TCB7);
	OS_addTask(&TCB8);
	OS_addTask(&TCB9);
	OS_addTask(&TCB10);
	
	/* Start the OS */
	OS_start();
	
}
