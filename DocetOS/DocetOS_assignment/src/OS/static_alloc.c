#include "OS/static_alloc.h"
#include <stdint.h>

/* Static Memory Allocation Partition */

// 16KB pool size
#define STATIC_ALLOC_POOLSIZE 16384UL
#define STATIC_ALLOC_ALIGNMENT 8UL

static uint8_t static_pool[STATIC_ALLOC_POOLSIZE] __attribute__ (( aligned(STATIC_ALLOC_ALIGNMENT) ));;
static size_t pool_index = STATIC_ALLOC_POOLSIZE;
static uint32_t bitmask = ~(STATIC_ALLOC_ALIGNMENT-1);

void * static_alloc(size_t bytes) {
	/* 
	* Check that the requested number of bytes is available in the array. This is 
	* easy because the pool array index decreases: if the index is greater than
	* the number of requested bytes, all is fine. If not, zero should be returned
	* to indicate failure.
	*/

	if(pool_index < bytes){
		return 0;
	}
	else{
		// Reduce the pool array index by the requested number of bytes.
		pool_index = (pool_index - bytes) & bitmask;
		
		// Return the new pool array index.
	return &static_pool[pool_index];
	}
}
