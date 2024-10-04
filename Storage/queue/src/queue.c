#include <stdio.h>
#include "queue.h"

int8_t queue_put(queue_t * queue, int32_t data) {
	queue->data[queue->insert] = data;
	queue->insert = (queue->insert + 1) % QUEUE_SIZE;
	return 0;
}

int32_t queue_get(queue_t * queue, int8_t * return_code) {
	if(queue->insert == queue->remove){
		*return_code = 0;
		printf("ERROR: Queue is empty!\r\nThere is nothing to get!\r\n");
		return 0;
	}
	else{
		int32_t data = queue->data[queue->remove];
		queue->remove = (queue->remove + 1) % QUEUE_SIZE;
		*return_code = 1;
		return data;
	}
}
