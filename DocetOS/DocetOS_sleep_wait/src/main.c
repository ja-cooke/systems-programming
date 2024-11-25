#include "OS/os.h"
#include "OS/sleep.h"
#include "Utils/utils.h"
#include "mode_utils.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

static void task1(void const *const args) {
	(void) args;
	
	while (1) {
		printf("AAAAAAAA");
		OS_sleep(100);
	}
}

static void task2(void const *const args) {
	(void) args;
	
	while (1) {
		printf("BBBBBBBB");
		OS_sleep(20);
	}
}

static void task3(void const *const args) {
	(void) args;
	
	while (1) {
		printf("CCCCCCCC");
	}
}

/* MAIN FUNCTION */

int main(void) {
	configClock();
	configUSART2(38400);
	
	reportState();
	
	printf("\r\nDocetOS\r\n");
	
	uint32_t test_func1;
	uint32_t test_func2;
	test_func1 = getPSR();
	test_func2 = getCONTROL();
	printf("Test: %"PRId32".\r\n", test_func1);
	printf("Test: %"PRId32".\r\n", test_func2);
	
	/* Reserve memory for two stacks and two TCBs.
	   Remember that stacks must be 8-byte aligned. */
	static uint32_t stack1[128] __attribute__ (( aligned(8) ));
	static uint32_t stack2[128] __attribute__ (( aligned(8) ));
	static uint32_t stack3[128] __attribute__ (( aligned(8) ));
	static OS_TCB_t TCB1, TCB2, TCB3;

	/* Initialise the TCBs using the two functions above */
	OS_initialiseTCB(&TCB1, stack1+128, task1, NULL);
	OS_initialiseTCB(&TCB2, stack2+128, task2, NULL);
	OS_initialiseTCB(&TCB3, stack3+128, task3, NULL);
	
	/* Add the tasks to the scheduler */
	OS_addTask(&TCB1);
	OS_addTask(&TCB2);
	//OS_addTask(&TCB3);
	
	/* Start the OS */
	OS_start();
	
}
