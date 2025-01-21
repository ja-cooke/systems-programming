#include <stdlib.h>

#include "tasks.h"

static OS_mutex_t mutex_p0 = OS_MUTEX_STATIC_INITIALISER;
static OS_mutex_t mutex_p1 = OS_MUTEX_STATIC_INITIALISER;
static OS_mutex_t mutex_p2 = OS_MUTEX_STATIC_INITIALISER;
static OS_mutex_t mutex_p3 = OS_MUTEX_STATIC_INITIALISER;
static OS_mutex_t mutex_p4 = OS_MUTEX_STATIC_INITIALISER;

/* Declare and intialise a memory pool */
static mempool_t pool = MEMPOOL_INITIALISER;

static uint32_t sleepID = 0;

void initialiseOSResources(void){
	pool_init(&pool, sizeof(packet_t), 10);
	OS_initCommsQueue();
	
	reportState();
	
	printf("\r\nDocetOS\r\n");
	
	uint32_t test_func1;
	uint32_t test_func2;
	test_func1 = getPSR();
	test_func2 = getCONTROL();
	printf("Test: %"PRId32".\r\n", test_func1);
	printf("Test: %"PRId32".\r\n", test_func2);
}



void task1(void const *const args) {
	(void) args;
	
	while (1) {
		OS_sleep(1000);
		
		// Inter-Task Communication Test
		// ---- SEND TEST ---------
		
		packet_t *commsTestPackTransmitter = pool_allocate(&pool);
		commsTestPackTransmitter->id = 2049;
		strncpy(commsTestPackTransmitter->data, "CommsTest", 10);
		
		OS_sendPacket(commsTestPackTransmitter, 1);
		
		// ---- SEND TEST END ---------
		
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
		
		OS_mutex_aquire(&mutex_p0);
		printf("--DO_NOT_INTERRUPT--");
		printf("First allocated packet (id %" PRIu32 ", data '%s') at address %p\r\n", p1->id, p1->data, (void *)p1);
		OS_mutex_release(&mutex_p0);
		
		// DEALLOCATION
		pool_deallocate(&pool, p1);
	}
	
}

void task2(void const *const args) {
	(void) args;
	
	while (1) {
		OS_sleep(1000);
		
		// ---- RECEIVE TEST ---------
	
		void **r1_ptr = OS_receivePacket(1);
		packet_t *r1 = (packet_t *) r1_ptr;
		
		OS_mutex_aquire(&mutex_p0);
		printf("--DO_NOT_INTERRUPT--");
		printf("Packet Received (id %" PRIu32 ", data '%s') at address %p\r\n", r1->id, r1->data, &r1->data);
		OS_mutex_release(&mutex_p0);
		
		pool_deallocate(&pool, r1_ptr);
	
		// ---- RECEIVE TEST END ---------
		
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
		OS_mutex_aquire(&mutex_p0);
		printf("--DO_NOT_INTERRUPT--");
		printf("Second allocated packet (id %" PRIu32 ", data '%s') at address %p\r\n", p2->id, p2->data, (void *)p2);
		OS_mutex_release(&mutex_p0);
		
		// DEALLOCATION
		pool_deallocate(&pool, p2);
	}
}

void task3(void const *const args) {
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
		OS_mutex_aquire(&mutex_p0);
		printf("--DO_NOT_INTERRUPT--");
		printf("Third allocated packet (id %" PRIu32 ", data '%s') at address %p\r\n", p3->id, p3->data, (void *)p3);
		OS_mutex_release(&mutex_p0);
		
		// DEALLOCATION
		pool_deallocate(&pool, p3);
		
	}
}

void sleepTest1(void const *const args) {
	(void) args;
	while(1) {
		
		uint32_t runtimeID = sleepID++;
		OS_mutex_aquire(&mutex_p1);
		printf("--DO_NOT_INTERRUPT--");
		uint32_t currentTime = OS_elapsedTicks();
		uint32_t sleepTime = 100;
		printf("ID : %"PRIu32". Sleep Time : %" PRIu32 ". Current Time : %"PRIu32".\r\n", runtimeID, sleepTime, currentTime);
		OS_mutex_release(&mutex_p1);
		
		OS_sleep(sleepTime);
		
		OS_mutex_aquire(&mutex_p1);
		printf("--DO_NOT_INTERRUPT--");
		currentTime = OS_elapsedTicks();
		printf("ID : %"PRIu32". Waking Up Time : %"PRIu32". Current Time : %"PRIu32".\r\n", runtimeID, sleepTime, currentTime);
		OS_mutex_release(&mutex_p1);
		
	}
}
void sleepTest2(void const *const args) {
	(void) args;
	while(1) {
		uint32_t runtimeID = sleepID++;
		OS_mutex_aquire(&mutex_p0);
		printf("\r\n--DO_NOT_INTERRUPT--");
		uint32_t currentTime = OS_elapsedTicks();
		uint32_t sleepTime = 3000;
		printf("ID : %"PRIu32". Sleep Time : %" PRIu32 ". Current Time : %"PRIu32".\r\n", runtimeID, sleepTime, currentTime);
		OS_mutex_release(&mutex_p0);
		
		OS_sleep(sleepTime);
		
		OS_mutex_aquire(&mutex_p0);
		printf("\r\n--DO_NOT_INTERRUPT--");
		currentTime = OS_elapsedTicks();
		printf("ID : %"PRIu32". Waking Up Time : %"PRIu32". Current Time : %"PRIu32".\r\n", runtimeID, sleepTime, currentTime);
		OS_mutex_release(&mutex_p0);
	}
}