#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "mode_utils.h"
#include "OS/os.h"
#include "OS/sleep.h"
#include "OS/mutex.h"
#include "Utils/utils.h"
#include "OS/mempool.h"

static OS_mutex_t mutex = OS_MUTEX_STATIC_INITIALISER;

/* Declare and intialise a memory pool */
static mempool_t pool = MEMPOOL_INITIALISER;

/* Example data packet structure, just for demonstration purposes */
typedef struct {
	uint32_t id;
	char data[12];
} packet_t;

static void task1(void const *const args) {
	(void) args;
	
	while (1) {
		OS_mutex_aquire(&mutex, OS_currentTCB());
		printf("--DO_NOT_INTERUPT-A--");
		OS_mutex_release(&mutex, OS_currentTCB());
	}
}

static void task2(void const *const args) {
	(void) args;
	
	while (1) {
		OS_mutex_aquire(&mutex, OS_currentTCB());
		printf("--DO_NOT_INTERUPT-B--");
		OS_mutex_release(&mutex, OS_currentTCB());
	}
}

static void task3(void const *const args) {
	(void) args;
	
	while (1) {
			OS_mutex_aquire(&mutex, OS_currentTCB());
			printf("--DO_NOT_INTERUPT-C--");
			OS_mutex_release(&mutex, OS_currentTCB());
	}
}

/* MAIN FUNCTION */

int main(void) {
	configClock();
	configUSART2(38400);
	
	pool_init(&pool, sizeof(packet_t), 10);
	
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
	OS_addTask(&TCB3);
	
	/* Start the OS */
	OS_start();
	
}
