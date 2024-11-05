#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "Utils/utils.h"

#pragma pack(push)
#pragma pack(1)

typedef struct {
	void (*callback) (void *);
	void *ptr;
	int32_t x;
	int32_t y;
} structure_t;

#pragma pack(pop)

void process(structure_t * s);

void printInteger(void * integer);
void printDouble(void * doububble);

int main(void) {
	configClock();
	configUSART2(38400);

	// 7.4.1
	
	structure_t blade_runner = {.x=20, .y=29};
	
	int16_t doomsday = 2808;
	
	blade_runner.ptr = &doomsday;
	blade_runner.callback = &printInteger;

	process(&blade_runner);

	printf("Blade Runner %d%d\r\n", blade_runner.x, blade_runner.y);
	
	// 7.4.2
	
	structure_t wipeout = {.x=20, .y=77};
	
	double doubleday = 2808;
	
	wipeout.ptr = &doubleday;
	wipeout.callback = &printDouble;

	process(&wipeout);

	printf("Wipeout %d%d\r\n", wipeout.x, wipeout.y);
	
	while(1);
}

void printInteger(void * integer) {
	int16_t * local_int = (int16_t*) integer;
	printf("The year is %" PRId16 "... Doomsday!\r\n", *local_int);
}

void printDouble(void * doububble) {
	double * local_double = (double*) doububble;
	printf("The year is %f... Doomsday!\r\n", *local_double);
}
