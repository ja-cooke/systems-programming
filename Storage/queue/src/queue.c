#include <stdio.h>
#include <inttypes.h>
#include "queue.h"

static int32_t spaces_left = 0;
static int32_t *spaces_left_ptr = &spaces_left;

void spaces(queue_t *queue){
	*spaces_left_ptr = (int32_t)(queue->remove - queue->insert) + QUEUE_SIZE - 1;
	printf("Spaces left: %" PRId32 "\r\n", *spaces_left_ptr);
}

int8_t queue_put(queue_t *queue, int32_t data) {
	spaces(queue);
	
	if(*spaces_left_ptr>0){
		queue->data[queue->insert] = data;
		queue->insert = (queue->insert + 1) % QUEUE_SIZE;
	}
	else{
		return 0;
	}
	return 1;
}

int32_t queue_get(queue_t *queue, int8_t *return_code) {	
	uint_fast16_t insert = queue->insert;
	uint_fast16_t remove = queue->remove;
	
	if(insert == remove){
		*return_code = 0;
		return 0;
	}
	else{
		int32_t data = queue->data[queue->remove];
		queue->remove = (queue->remove + 1) % QUEUE_SIZE;
		*return_code = 1;
		return data;
	}
}
