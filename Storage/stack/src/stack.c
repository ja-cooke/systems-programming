#include <stdio.h>
#include "stack.h"

void push(int32_t **sp, int32_t value) {
	// Increment the stack value
	*sp = *(sp + 1);
	// Add the value to the top of the stack
	**sp = value;
}

int32_t pop(int32_t **sp) {
	int32_t value;

	// Read the value at the top of the stack 
	value = **sp;
	// Decrement the stack
	*sp = *(sp - 1);
	printf("*sp = %" PRIdPTR "\r\n", **sp);
	
	return value;
}
