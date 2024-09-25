#include "Utils/utils.h"
#include <stdio.h>

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	void square(long long *value);
	
	long long i = 3;
	
	printf("---RESET---\r\n\r\n");
	
	printf("Memory Address of i: (&i) = 0x%p \r\n", &i);
	printf("(&i) + 1 = 0x%p \r\n", (void *)((&i) + 1));
	
	printf("i = %lld\r\n", i);
	printf("square(&i)\r\n");
	square(&i);
	printf("i = %lld\r\n", i);
	
	while(1);
}

void square(long long *value){
	*value = *value**value;
}
