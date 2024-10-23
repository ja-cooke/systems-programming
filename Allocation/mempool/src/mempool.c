#include "mempool.h"
#include <inttypes.h>

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

void *pool_allocate(mempool_t *pool) {
	void * output;
	
	// Return the head of the list of blocks
	output = pool->head;
	
	// Update the head pointer
	if (pool->head){
  pool->head = pool->head->next;
	}
	else{
		return 0;
	}
	
	return output;
}

void pool_deallocate(mempool_t *pool, void *block) {
	// Add the new item to the head of the list
	
	// Cast the void to int to hold pointer value.
	mempool_item_t * block_cast = block;
	
	// Insert a pointer into block that points to the current head of the list	
	block_cast->next = pool->head;
	
	// Update the head pointer to point to the new block.
	pool->head = block;
}

void pool_init(mempool_t *pool, size_t blocksize, size_t blocks){
	
	// Round blocksize up to nearest multiple of 8 (one byte)
	uint32_t bitmask = ~(STATIC_ALLOC_ALIGNMENT-1);
	blocksize = (blocksize + 7UL) & bitmask;
	
	size_t pool_index = 0;
	pool->head = static_alloc(blocksize*blocks);
	
	
	if(pool->head){
		for (size_t i = 0; i < blocks; ++i) {
			pool_index = i * blocksize;
			pool_add(pool, &pool[pool_index]);
		}
	}
	else{
		pool->head = 0;
	}
	
	
	
}
