#include "Utils/utils.h"
#include <stdio.h>
#include "stack.h"

int main(void) {
	
	configClock();
	configUSART2(38400);
		
	// Array to hold stack entries
	int32_t stack[10];
	// Stack pointer, initially points to the first array element
	int32_t *stack_p = stack;

	int32_t result;
	for(int32_t i = 0; i < 10; i++){
		push(&stack_p, i);
		printf("Stack pointer value = %" PRId32 "\r\n", *stack_p);
	}
	
	for(int32_t i = 0; i < 10; i++){
		result = pop(&stack_p);
		printf("Stack pointer value = %" PRId32 "\r\n", *stack_p);
		printf("Result of pop = %" PRId32 "\r\n", result);
	}
	
	while(1);
}
