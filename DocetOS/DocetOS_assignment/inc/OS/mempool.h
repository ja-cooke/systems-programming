#ifndef MEMPOOL_H
#define MEMPOOL_H
#include <stddef.h>
#include "cmsis_compiler.h"
#include "OS/semaphore.h"

typedef struct s_mempool_item {
	struct s_mempool_item *next;
} mempool_item_t;

typedef struct s_mempool {
	mempool_item_t *head;
	OS_semaphore_t *blocksFree;
} mempool_t;

#define MEMPOOL_INITIALISER { .head = 0 }

void * static_alloc(size_t bytes);

void * pool_allocate(mempool_t *pool);
void pool_deallocate(mempool_t *pool, void *block);
void pool_init(mempool_t *pool, size_t blocksize, size_t blocks);
	
#define pool_add pool_deallocate

#endif /* MEMPOOL_H */
