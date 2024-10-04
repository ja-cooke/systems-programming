#include "Utils/utils.h"
#include <stdio.h>
#include "stack.h"

int main(void) {
	
	configClock();
	configUSART2(38400);
		
	printf("\r\n --- Restarting --- \r\n\r\n"); 
	
	// Array to hold stack entries
	int32_t stack[10];
	// Stack pointer, initially points to the first array element
	int32_t *stack_p = stack;

	int32_t result;
	for(int32_t i = 0; i < 10; i++){
		printf("Pushing = %" PRId32 "\r\n", i);
		push(&stack_p, i);
	}
	
	for(int32_t i = 0; i < 10; i++){
		result = pop(&stack_p);
		printf("Popped = %" PRId32 "\r\n", result);
	}
	
	while(1);
}
