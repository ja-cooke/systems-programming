#include "Utils/utils.h"
#include <stdio.h>

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	printf("---RESET---\r\n");
	
	printf("bits: %d\r\n", sizeof(char)*8);
	
	int x = 42000;
	short int y;
	
	y = (short int) x;
	
	printf("x = %d, y = %hd\r\n", x, y);
	
	printf("---Literals---\r\n");
	
	unsigned int mask = (0xBAUL) << 24;
	printf("mask: 0x%08X\r\n", mask);

	while(1);
}
