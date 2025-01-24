#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include "semaphore.h"

typedef struct {
	void ** const store;
	uint32_t size;
	int32_t (*comparator) (void *, void *);
	OS_semBinary_t accessToken;
} OS_heap_t;

#define HEAP_INITIALISER(arg_store, arg_comparator, arg_accessToken) { .store = (arg_store), .size = 0, .comparator = (arg_comparator), .accessToken = arg_accessToken }

void heap_insert(OS_heap_t *heap, void *value);
void *heap_extract(OS_heap_t *heap);
uint32_t heap_isEmpty(OS_heap_t *heap);

#endif /* HEAP_H */
