#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "Utils/utils.h"

int main(void) {
	configClock();
	configUSART2(38400);

//	uint32_t x=3, y=5, z =0;
//  __asm volatile (
//    "ADD %[output1], %[input1], %[input1], LSL #3\n\t" /* x*9 */
//    "ADD %[output1], %[output1], %[input1], LSL #2\n\t" /* +(x*4) */
//		"ADD %[output2], %[output1], %[input2]" /* 13x + y */
//    : [output1] "+&r" (y), [output2] "=&r" (z)
//    : [input1] "r" (x), [input2] "r" (y)
//	);
	
		uint32_t x=3, y=5;
  __asm volatile (
    "ADD r4, %[input1], %[input1], LSL #3\n\t" /* x*9 */
    "ADD r4, r4, %[input1], LSL #2\n\t" /* +(x*4) */
		"ADD %[output1], r4, %[input2]" /* 13x + y */
    : [output1] "=&r" (y)
    : [input1] "r" (x), [input2] "r" (y)
		: "r4"
	);

	printf("y = %" PRIu32 "\n", y);

	while(1);
}
