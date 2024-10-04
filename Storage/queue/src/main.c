#include "Utils/utils.h"
#include "queue.h"
#include <inttypes.h>
#include <stdio.h>

queue_t queue = QUEUE_INITIALISER;

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	printf("\r\n --RESTART-- \r\n");
	
	for(int32_t i = 0; i < 10; i++) {
		queue_put(&queue, i);
	}
	
	for(int32_t i = 0; i < 10; i++) {
		int8_t return_code;
		return_code = 1;
		int32_t dequeued_value;
		dequeued_value = queue_get(&queue, &return_code);
		
		if(return_code){
			printf("Removed %" PRId32 " from queue\r\n", dequeued_value);
		}
		else{
			printf("Dequeue Failure \r\n");
		}
	}
}
