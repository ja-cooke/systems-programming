#include "mempool.h"
#include "static_alloc.h"
#include <inttypes.h>

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
	
	size_t pool_index = 0;
	pool = static_alloc(blocksize*blocks);
	//static uint32_t bitmask = ~(7UL);
	
	if(pool){
		for (size_t i = 0; i < blocks; ++i) {
			//pool_index = (pool_index - bytes) & bitmask;
			pool_index = i * blocksize;
			pool_add(pool, &pool[pool_index]);
		}
	}
	else{
		pool->head = 0;
	}
	
	
	
}
