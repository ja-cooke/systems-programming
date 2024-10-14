#include "static_alloc.h"
#include <stdint.h>

// 16KB pool size
#define STATIC_ALLOC_POOLSIZE 16384UL

static uint8_t static_pool[STATIC_ALLOC_POOLSIZE];
static size_t pool_index = STATIC_ALLOC_POOLSIZE;

void * static_alloc(size_t bytes) {
	/* 
	* Check that the requested number of bytes is available in the array. This is 
	* easy because the pool array index decreases: if the index is greater than
	* the number of requested bytes, all is fine. If not, zero should be returned
	* to indicate failure.
	*/
	
	size_t * pool_index_ptr = &pool_index;
	size_t callpoint_pool_index = *pool_index_ptr;
	
	if(pool_index < bytes){
		return 0;
	}
	else{
		// Reduce the pool array index by the requested number of bytes.
		*pool_index_ptr = *pool_index_ptr - bytes;
		
		// Return the new pool array index.
	return pool_index_ptr;
	}
}
