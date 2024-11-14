#include "Utils/utils.h"
#include <stdio.h>

static void test(void) {
	printf("Hello!\r\n");
}

int main(void) {
	configClock();
	configUSART2(38400);

	printf("---RESET---\r\n");
	test();
	printf("---Overwite Test---\r\n");
	
	uint32_t array[10];
	void * test_ptr = *test;
		
	// First try overflowing the stack with garbage and see what happens
	// Then try to overwrite the link register here so that test() is run
	array[0]=0;
	array[1]=0;
	array[2]=2;
	array[3]=3;
	array[4]=4;
	array[5]=5;
	array[6]=6;
	array[7]=7;
	array[8]=8;
	array[9]=9;
	
	*(array+11)= (uint32_t) test_ptr; // This causes the most problems
}