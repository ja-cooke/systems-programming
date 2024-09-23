#include "Utils/utils.h"
#include <stdio.h>

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	printf("char: %d\r\n", sizeof(char));
	
	int x = 42;
	unsigned int y = 42;
	
	y = x;
	(void) y;
	
	while(1);
}
