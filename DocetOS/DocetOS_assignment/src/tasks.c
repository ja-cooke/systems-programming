#include <stdlib.h>

#include "tasks.h"
#include "OS/static_alloc.h"

/* 
 * DEMONSTRATION CODE
 *
 * Not all of the tasks provided here are used by the program as some were
 * built for test / debugging purposes.
 */

/* Seperate Mutexes are required for each task priority level */
static OS_mutex_t mutex_p0 = OS_MUTEX_STATIC_INITIALISER;
static OS_mutex_t mutex_p1 = OS_MUTEX_STATIC_INITIALISER;
static OS_mutex_t mutex_p2 = OS_MUTEX_STATIC_INITIALISER;
static OS_mutex_t mutex_p3 = OS_MUTEX_STATIC_INITIALISER;
static OS_mutex_t mutex_p4 = OS_MUTEX_STATIC_INITIALISER;

/* Declare and intialise a memory pool */
static mempool_t pool = MEMPOOL_INITIALISER;
static uint32_t const poolSize = 100;

/* Used to check that sleeping tasks are waking in the correct order */
static uint32_t sleepID = 0;

/* Dynamic memory pools must be initialised prior to starting the OS. */
void initialiseOSResources(void){
	pool_init(&pool, sizeof(packet_t), poolSize);
	OS_initCommsQueue();
	
	printf("\r\n -- DocetOS -- \r\n");
}

/* This tesk was used to check the Sorting from the Sleep Heep */
void sleepTest1(void const *const args) {
	(void) args;
	
	while(1) {
		uint32_t runtimeID = sleepID++;
		OS_mutex_acquire(&mutex_p0);
		printf("--DO_NOT_INTERRUPT--");
		uint32_t currentTime = OS_elapsedTicks();
		uint32_t sleepTime = 100;
		printf("ID : %"PRIu32". Sleep Time : %" PRIu32 ". Current Time : %"PRIu32".\r\n", runtimeID, sleepTime, currentTime);
		OS_mutex_release(&mutex_p0);
		
		OS_sleep(sleepTime);
		
		OS_mutex_acquire(&mutex_p0);
		printf("--DO_NOT_INTERRUPT--");
		currentTime = OS_elapsedTicks();
		printf("ID : %"PRIu32". Waking Up Time : %"PRIu32". Current Time : %"PRIu32".\r\n", runtimeID, sleepTime, currentTime);
		OS_mutex_release(&mutex_p0);
	}
}

/* This tesk was used to check the Sorting from the Sleep Heep */
void sleepTest2(void const *const args) {
	(void) args;
	
	while(1) {
		uint32_t runtimeID = sleepID++;
		
		OS_mutex_acquire(&mutex_p0);
		printf("\r\n--DO_NOT_INTERRUPT--");
		uint32_t currentTime = OS_elapsedTicks();
		uint32_t sleepTime = 3000;
		printf("ID : %"PRIu32". Sleep Time : %" PRIu32 ". Current Time : %"PRIu32".\r\n", runtimeID, sleepTime, currentTime);
		OS_mutex_release(&mutex_p0);
		
		OS_sleep(sleepTime);
		
		OS_mutex_acquire(&mutex_p0);
		printf("\r\n--DO_NOT_INTERRUPT--");
		currentTime = OS_elapsedTicks();
		printf("ID : %"PRIu32". Waking Up Time : %"PRIu32". Current Time : %"PRIu32".\r\n", runtimeID, sleepTime, currentTime);
		OS_mutex_release(&mutex_p0);
	}
}

/* This task shows that tasks can send memory blocks to other tasks */
void chattyTask(void const *const args) {
	(void) args;
	
	while (1) {
		OS_sleep(5000);
		for(uint32_t i = 0; i<5; i++) {
			OS_sleep(1000);
			
			/* This task must allocate
			 * but never deallocate a memory block it has sent */
			uint32_t randomID = (uint32_t) rand();
			packet_t *commsTestPackTransmitter = pool_allocate(&pool);
			commsTestPackTransmitter->id = randomID;
			strncpy(commsTestPackTransmitter->data, "HelloWorld", 10);
			
			OS_mutex_acquire(&mutex_p3);
			printf("Sending on Channel 1\r\n");
			printf("Sending Packet: (id %" PRIu32 ", data '%s') at address %p\r\n", randomID, commsTestPackTransmitter->data, &commsTestPackTransmitter->data);
			OS_mutex_release(&mutex_p3);
			OS_sendPacket(commsTestPackTransmitter, 1);
			
			/* Similar message send on a different channel */
			randomID = (uint32_t) rand();
			commsTestPackTransmitter = pool_allocate(&pool);
			commsTestPackTransmitter->id = randomID;
			strncpy(commsTestPackTransmitter->data, "HalloWelt", 10);
			
			OS_mutex_acquire(&mutex_p3);
			printf("Sending on Channel 5\r\n");
			printf("Sending Packet: (id %" PRIu32 ", data '%s') at address %p\r\n", randomID, commsTestPackTransmitter->data, &commsTestPackTransmitter->data);
			OS_mutex_release(&mutex_p3);
			OS_sendPacket(commsTestPackTransmitter, 5);
		}
	}
}


/* This task shows that tasks can receive memory blocks from other tasks */
void listeningTask(void const *const args) {
	(void) args;
	
	OS_sleep(500);
		
	while(1) {
		
		OS_sleep(5000);
			
		for(uint32_t i = 0; i<5; i++) {
			OS_sleep(1000);
		
			OS_mutex_acquire(&mutex_p3);
			printf("Listening on Channel 1\r\n");
			OS_mutex_release(&mutex_p3);
			
			void **r1_ptr = OS_receivePacket(1);
			packet_t *r1 = (packet_t *) r1_ptr;
			
			OS_mutex_acquire(&mutex_p3);
			printf("Packet Received (id %" PRIu32 ", data '%s') at address %p\r\n", r1->id, r1->data, &r1->data);
			OS_mutex_release(&mutex_p3);
			
			/* The receiving task MUST deallocate data it receives when it is 
			 * finished using it. This will/should not be done by the sending task. */
			pool_deallocate(&pool, r1_ptr);
			
			OS_mutex_acquire(&mutex_p3);
			printf("Listening on Channel 5\r\n");
			OS_mutex_release(&mutex_p3);
			
			r1_ptr = OS_receivePacket(5);
			r1 = (packet_t *) r1_ptr;
			
			OS_mutex_acquire(&mutex_p3);
			printf("Packet Received (id %" PRIu32 ", data '%s') at address %p\r\n", r1->id, r1->data, &r1->data);
			OS_mutex_release(&mutex_p3);
			
			pool_deallocate(&pool, r1_ptr);
		}
	}
}

/* This task showcases allocation using the dynamic memory pool */
void greedyTask(void const *const args) {
	(void) args;
	
	#define numItems 90UL
	
	OS_sleep(15000);
	
	while(1) {
		
		packet_t *memoryTest[numItems] = {0};
		
		for(uint32_t i = 0; i<numItems; i++){
			// ALLOCATION //
			/* Allocate one block for data packets and fill them in */
			memoryTest[i] = pool_allocate(&pool);
			memoryTest[i]->id = i;
			strncpy(memoryTest[i]->data, "IMPORTANT", 10);
			
			// PRINTING //
			OS_mutex_acquire(&mutex_p2);
			printf("--DO_NOT_INTERRUPT--");
			printf("Allocated Memory (id %" PRIu32 ", data '%s') at address %p\r\n", memoryTest[i]->id, memoryTest[i]->data, (void *)memoryTest[i]);
			OS_mutex_release(&mutex_p2);
			OS_sleep(100);
		}
		
		OS_sleep(10000);
		
		for(uint32_t i = 0; i<numItems; i++) {
			pool_deallocate(&pool, memoryTest[i]);
		}
	}
}

/* Designed to be a very noticeable interruption */
void veryHighPriorityTask(void const *const args) {
	(void) args;
	
	while(1) {
		OS_sleep(20000);
		for (uint32_t i = 0; i<100; i++) {
			printf("\r\nSYSTEM PANIC\r\n");
		}
		printf("\r\nsystem calm\r\n");
	}
}

/* A simple clock */
void lowPriorityTask(void const *const args) {
	(void) args;
	
	while(1) {
		OS_mutex_acquire(&mutex_p4);
		uint32_t currentTime = OS_elapsedTicks();
		printf("System Time : %"PRIu32".\r\n", currentTime);
		OS_mutex_release(&mutex_p4);
		OS_sleep(1000);
	}
}