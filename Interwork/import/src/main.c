#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "Utils/utils.h"

void fib(uint32_t n);

void report(uint32_t value) {
	printf("%" PRIu32 "\r\n", value);
}

int main(void) {
	configClock();
	configUSART2(38400);
	
	for(uint32_t i=0; i<40; i++){
		fib(i);
	}

	while(1);
}
