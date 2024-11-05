#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "Utils/utils.h"

#pragma pack(push)
#pragma pack(1)

typedef struct {
	int32_t x;
	int32_t y;
	void *ptr;
	void (*callback) (void *);
} structure_t;

#pragma pack(pop)

void process(structure_t * s);

void printInteger(void * integer);

int main(void) {
	configClock();
	configUSART2(38400);

	structure_t blade_runner = {.x=20, .y=49};
	
	int16_t doomsday = 2808;
	
	blade_runner.ptr = &doomsday;
	blade_runner.callback = &printInteger;

	process(&blade_runner);

	printf("Value of x: %d -- Value of y: %d\r\n", blade_runner.x, blade_runner.y);
	
	while(1);
}

void printInteger(void * integer) {
	int16_t * local_int = (int16_t*) integer;
	printf("The value of the integer is: %" PRId16 "\r\n", *local_int);
}
