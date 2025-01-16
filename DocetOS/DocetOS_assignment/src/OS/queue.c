#include <stdio.h>
#include <inttypes.h>
#include "OS/queue.h"
#include "OS/semaphore.h"

static int32_t spaces_left = 0;
static int32_t *spaces_left_ptr = &spaces_left;

static OS_commsQueue_t commsQueue = QUEUE_INITIALISER;

void OS_initCommsQueue(void) {
	commsQueue.accessToken.available = 1;
}

void OS_sendPacket(void *packet, uint32_t commID) {
	// DANGER ZONE
	uint32_t sendError = 0;
	
	OS_commsPacket_t packetPacked = PACKET_INITIALISER;
	packetPacked.memoryAddress = packet;
	packetPacked.commID = commID;
	
	do {
		sendError = queue_put(&commsQueue, packetPacked);
	} while(sendError);
	// DANGER ZONE
}

void * OS_receivePacket(uint32_t commID) {
	// DANGER ZONE
  uint32_t notReceived = 1;
	OS_commsPacket_t packetPacked = PACKET_INITIALISER;
	
	/* 
	 * Listens for the packet.
	 * Sets the task to wait if the packet is not yet available.
	 */
	while(notReceived) {
		uint32_t notification_counter = getNotificationCounter();
		notReceived = queue_get(&commsQueue, &packetPacked, commID);
		if(notReceived) {
			OS_wait(notification_counter);
		}
	}
	
	/* Unpacks packet data pointer */
	void *packet = packetPacked.memoryAddress;
	
	return packet;
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

uint32_t queue_get(OS_commsQueue_t *queue, OS_commsPacket_t *packet, uint32_t commID) {	
	// DANGER ZONE
	OS_semBinary_acquire(&queue->accessToken);
	
	uint32_t insert = queue->insert;
	uint32_t remove = queue->remove;
	
	/* Queue is empty, nothing to get */
	if(insert == remove){ 
		OS_semBinary_release(&queue->accessToken);
		return 1;
	}
	/* 
	 * If the queue is not empty, and the commID matches the one expected, 
	 * retrieve it. 
	 */
	else if (queue->packets[queue->remove].commID == commID){
		*packet = queue->packets[queue->remove];
		queue->remove = (queue->remove + 1) % QUEUE_SIZE;
	
		/* Queue get successful */
		OS_semBinary_release(&queue->accessToken);
		OS_notifyAll();
		return 0;
	}
	/*
	 * If the queue is not empty, but the commID does not match.
	 */
	else {
		OS_semBinary_release(&queue->accessToken);
		return 1;
	}
	// DANGER ZONE
}
