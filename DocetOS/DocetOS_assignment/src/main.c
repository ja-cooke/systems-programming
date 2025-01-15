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
		
		for(uint32_t i = 0; i<100000; i++){
			// ALLOCATION //
			/* Allocate one block for data packets and fill them in */
			packet_t *p1 = pool_allocate(&pool);
			p1->id = 1;
			strncpy(p1->data, "AAA", 10);
			pool_deallocate(&pool, p1);
		}
		
		// ALLOCATION //
		/* Allocate one block for data packets and fill them in */
		packet_t *p1 = pool_allocate(&pool);
		p1->id = 1;
		strncpy(p1->data, "AAA", 10);
		
		OS_mutex_aquire(&mutex, OS_currentTCB());
		printf("--DO_NOT_INTERRUPT--");
		printf("First allocated packet (id %" PRIu32 ", data '%s') at address %p\r\n", p1->id, p1->data, (void *)p1);
		OS_mutex_release(&mutex, OS_currentTCB());
		
		// DEALLOCATION
		pool_deallocate(&pool, p1);
	}
}

static void task2(void const *const args) {
	(void) args;
	
	while (1) {
		
		for(uint32_t i = 0; i<10000; i++){
			// ALLOCATION //
			/* Allocate one block for data packets and fill them in */
			packet_t *p2 = pool_allocate(&pool);
			p2->id = 2;
			strncpy(p2->data, "BBB", 10);
			pool_deallocate(&pool, p2);
		}
		
		// ALLOCATION //
		/* Allocate one block for data packets and fill them in */
		packet_t *p2 = pool_allocate(&pool);
		p2->id = 2;
		strncpy(p2->data, "BBB", 10);
		
		
		// PRINTING //
		OS_mutex_aquire(&mutex, OS_currentTCB());
		printf("--DO_NOT_INTERRUPT--");
		printf("Second allocated packet (id %" PRIu32 ", data '%s') at address %p\r\n", p2->id, p2->data, (void *)p2);
		OS_mutex_release(&mutex, OS_currentTCB());
		
		// DEALLOCATION
		pool_deallocate(&pool, p2);
	
	}
}

static void task3(void const *const args) {
	(void) args;
	
	while (1) {
		
		for(uint32_t i = 0; i<110000; i++){
			// ALLOCATION //
			/* Allocate one block for data packets and fill them in */
			packet_t *p3 = pool_allocate(&pool);
			p3->id = 3;
			strncpy(p3->data, "CCC", 10);
			pool_deallocate(&pool, p3);
		}
		
		// ALLOCATION //
		/* Allocate one block for data packets and fill them in */
		packet_t *p3 = pool_allocate(&pool);
		p3->id = 3;
		strncpy(p3->data, "CCC", 10);
		
		// PRINTING //
		OS_mutex_aquire(&mutex, OS_currentTCB());
		printf("--DO_NOT_INTERRUPT--");
		printf("Third allocated packet (id %" PRIu32 ", data '%s') at address %p\r\n", p3->id, p3->data, (void *)p3);
		OS_mutex_release(&mutex, OS_currentTCB());
		
		// DEALLOCATION
		pool_deallocate(&pool, p3);
		
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
	static OS_TCB_t TCB1 = {.priority = 5};
	static OS_TCB_t TCB2 = {.priority = 4};
	static OS_TCB_t TCB3 = {.priority = 3};

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
