#include <inttypes.h>

#include "OS/mempool.h"

// 16KB pool size
#define STATIC_ALLOC_POOLSIZE 16384UL
#define STATIC_ALLOC_ALIGNMENT 8UL

static const uint32_t totalBlocks = 16384/8;

static uint8_t static_pool[STATIC_ALLOC_POOLSIZE] __attribute__ (( aligned(STATIC_ALLOC_ALIGNMENT) ));;
static size_t pool_index = STATIC_ALLOC_POOLSIZE;
static uint32_t alignmentMask = ~(STATIC_ALLOC_ALIGNMENT-1);

void * static_alloc(size_t bytes) {
	/* 
	* Check that the requested number of bytes is available in the array. This is 
	* easy because the pool array index decreases: if the index is greater than
	* the number of requested bytes, all is fine. If not, zero should be returned
	* to indicate failure.
	*
	* This function is run once prior to OS_start() so synchronisation is not 
	* required.
	*/

	if(pool_index < bytes){
		return 0;
	}
	else{
		// Reduce the pool array index by the requested number of bytes.
		pool_index = (pool_index - bytes) & alignmentMask;
		
		// Return the new pool array index.
	return &static_pool[pool_index];
	}
}

void * pool_allocate(mempool_t *pool) {
	uint32_t repeat = 0;
	void *allocatedBlock = 0;
	
	OS_semaphore_acquire(pool->blocksFree);
	
	do {
		// Return the head of the list of blocks
		//Equivalent to: allocatedBlock = pool->head
		allocatedBlock = (void *) __LDREXW((uint32_t *)&(pool->head));
		
		// Update the head pointer
		if (pool->head){
			// Equivalent to pool->head = pool->head->next
			repeat = __STREXW ((uint32_t) pool->head->next, (uint32_t *)&(pool->head));
		}
		else{
			// Allocation failed, return empty pointer
			return 0;
		}
	} while(repeat);
	
	// Allocation successful, return pointer to allocated block
	return allocatedBlock;
}

void pool_deallocate(mempool_t *pool, void *block) {
	// Add the new item to the head of the list
	
	// Cast the void to int to hold pointer value.
	mempool_item_t *block_ptr = block;
	
	do {
		// Insert a pointer into block that points to the current head of the list	
		// Equivalent to block_ptr->next = pool->head
		block_ptr->next = (mempool_item_t *)__LDREXW((uint32_t *)&(pool->head));
	
		// Update the head pointer to point to the new block.
		// Equivalent to pool->head = block
	} while(__STREXW((uint32_t)block, (uint32_t *)&(pool->head)));
	
	OS_semaphore_release(pool->blocksFree);
}

void pool_init(mempool_t *pool, size_t blocksize, size_t blocks){
	
	/* A counting semaphore keeps a thread-safe record of the number of 
	 * available memory blocks at all times.
	 */
	static OS_semaphore_t blocksFree = OS_SEMAPHORE_STATIC_INITIALISER;
	blocksFree.available = totalBlocks - (uint32_t)blocks;
	
	// Round blocksize up to nearest multiple of 8 (one byte)
	blocksize = (blocksize + 7UL) & alignmentMask;
	
	size_t pool_index = 0;
	pool->head = static_alloc(blocksize*blocks);
	
	/* If the static_alloc fails pool->head = 0 */
	if(pool->head){
		/* Creates an array of block indices and adds this to the pool list*/
		for (size_t i = 0; i < blocks; ++i) {
			pool_index = i * blocksize;
			pool_add(pool, &pool[pool_index]);
		}
		pool->blocksFree = &blocksFree;
	}
	else{
		blocksFree.available = 0;
		pool->blocksFree = &blocksFree;
	}
}
