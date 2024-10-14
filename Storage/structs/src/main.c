#include "Utils/utils.h"
#include <stdio.h>
#include <inttypes.h>

#pragma pack(push)
#pragma pack(1)

typedef struct {
	int8_t x;
	int32_t y;
} test_t;

#pragma pack(pop)

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	test_t test;
	test = (test_t) { .x = 70, .y = -6};
	
	// 4.3.2
	printf("Size of test_t = %d\r\n", sizeof(test));
	printf("test.x = %" PRId8 "\r\n", test.x);
	printf("test.y = %" PRId32 "\r\n", test.y);
	
	// 4.3.3
	test_t* test_p;
	test_p = &test;
	
	test_p->x = 5;
	test_p->y = 20;
	
	
	
	printf("test_p.x = %" PRId8 "\r\n", test.x);
	printf("test_p.y = %" PRId32 "\r\n", test.y);
	
	while(1);
}
