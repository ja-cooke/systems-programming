#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include "OS/scheduler.h"
#include "OS/semaphore.h"

#define QUEUE_SIZE 2048

typedef struct {
	void *memoryAddress;
	OS_TCB_t *sender;
	OS_TCB_t *receiver;
} OS_commsPacket_t;

typedef struct {
	OS_commsPacket_t packets[QUEUE_SIZE];
	OS_semBinary_t accessToken;
	uint32_t insert;
	uint32_t remove;
} OS_commsQueue_t;

#define QUEUE_INITIALISER { .packets = {0}, .insert = 0, .remove = 0 }
#define PACKET_INITIALISER { .memoryAddress = 0, .sender = 0, .receiver = 0 }

void OS_sendPacket(void *packet, OS_TCB_t *receiver);
void OS_receivePacket(void **packet, OS_TCB_t *sender);

void spaces(OS_commsQueue_t *queue);
uint32_t queue_put(OS_commsQueue_t *queue, OS_commsPacket_t packet);
uint32_t queue_get(OS_commsQueue_t *queue, OS_commsPacket_t *packet);

#endif /* QUEUE_H */
