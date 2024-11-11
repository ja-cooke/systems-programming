#include "Utils/utils.h"
#include <stdio.h>

static void test(void) {
	printf("Hello!\r\n");
}

int main(void) {
	configClock();
	configUSART2(38400);

	printf("---RESET---\r\n");
	printf("Test() test\r\n");
	test();
	printf("---Overwite Test---\r\n");
	
	uint32_t array[10];
	
	for(uint32_t i=0;i<10;i++){
		
	array[i]=i+1;
	// First try overflowing the stack with garbage and see what happens
	// Then try to overwrite the link register here so that test() is run

	}
	
	//*(array+9)= 9;
	//*(array+10)= 9;
	//*(array+11)= 9;
	*(array+10)= 34240008; // This causes the most problems
	//*(array+13)= 9;
	//*(array+14)= 9;
	
}