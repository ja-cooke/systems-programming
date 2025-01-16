#include <stdio.h>
#include <inttypes.h>
#include "OS/queue.h"
#include "OS/semaphore.h"

static int32_t spaces_left = 0;
static int32_t *spaces_left_ptr = &spaces_left;

static OS_commsQueue_t commsQueue = QUEUE_INITIALISER;

void OS_sendPacket(void *packet, OS_TCB_t *receiver) {
	// DANGER ZONE
	OS_TCB_t * current_tcb = OS_currentTCB();
	uint32_t sendError = 0;
	
	OS_commsPacket_t packetPacked = PACKET_INITIALISER;
	packetPacked.memoryAddress = &packet;
	packetPacked.sender = current_tcb;
	packetPacked.receiver = receiver;
	
	do {
		sendError = queue_put(&commsQueue, packetPacked);
	} while(sendError);
	// DANGER ZONE
}

void OS_receivePacket(void **packet, OS_TCB_t *sender) {
	// DANGER ZONE
  uint32_t receiveError = 1;
	OS_commsPacket_t packetPacked = PACKET_INITIALISER;
	
	while(receiveError) {
		uint32_t notification_counter = getNotificationCounter();
		receiveError = queue_get(&commsQueue, &packetPacked);
		if (receiveError) {
			OS_wait(notification_counter);
		}
	}
	/* Other tasks may be waiting for the next item in the queue */
	OS_notifyAll();
	
	packet = packetPacked.memoryAddress;
	sender = packetPacked.sender;
	
	(void) packet;
	(void) sender;
	// DANGER ZONE
}

/*--------------------------------------------------------------------------- */ 
/*------------------------------ QUEUE FUNCTIONS ---------------------------- */ 
/*--------------------------------------------------------------------------- */
/*------------------------------ Internal Only ------------------------------ */

void spaces(OS_commsQueue_t *queue){
	*spaces_left_ptr = (int32_t)(queue->remove - queue->insert) + QUEUE_SIZE - 1;
}

uint32_t queue_put(OS_commsQueue_t *queue, OS_commsPacket_t packet) {
	OS_semBinary_acquire(&queue->accessToken);
	
	spaces(queue);
	
	// DANGER ZONE
	if(*spaces_left_ptr>0){
		queue->packets[queue->insert] = packet;
		queue->insert = (queue->insert + 1) % QUEUE_SIZE;
		
		/* Success */
		OS_semBinary_release(&queue->accessToken);
		return 0;
	}
	else{
		/* Falure */
		OS_semBinary_release(&queue->accessToken);
		return 1;
	}
	// DANGER ZONE
}

uint32_t queue_get(OS_commsQueue_t *queue, OS_commsPacket_t *packet) {	
	// DANGER ZONE
	
	OS_semBinary_acquire(&queue->accessToken);
	
	uint32_t insert = queue->insert;
	uint32_t remove = queue->remove;
	
	/* Queue is empty, nothing to get */
	if(insert == remove){ 
		OS_semBinary_release(&queue->accessToken);
		return 1;
	}
	/* If the queue is not empty, check to see if the next item in the queue is
   * intended for the current TCB, and if it is, retrieve it. */
	else if (&queue->packets[queue->packets[queue->remove].receiver == OS_currentTCB()]){
		*packet = queue->packets[queue->remove];
		queue->remove = (queue->remove + 1) % QUEUE_SIZE;
	
		/* Queue get successful */
		OS_semBinary_release(&queue->accessToken);
		return 0;
	}
	/* If the queue is not empty, but the next item in the queue is intended for a
	 * different TCB */
	else {
		OS_semBinary_release(&queue->accessToken);
		return 1;
	}
	// DANGER ZONE
}
