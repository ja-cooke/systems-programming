#include "heap.h"

static void heap_up(heap_t *heap) {
	// 1. Start with the last element in the heap
	
	int32_t * const offset_store = heap->store - 1;
	
	int32_t * const newest_element = &offset_store[heap->size];
	
	// 2. If it's the root element, stop
	if(heap->size != 1){
		while(1){
			int32_t * const parent = newest_element;
			parent = parent >> 1;
			
			// 3. Compare it with its parent
			if(*newest_element <= *parent){
				// 4. If the parent is smaller or equal, stop
				return;
			}
			else{
				// 5. Swap the element with its parent
				int32_t swap_buffer;
				swap_buffer = *parent;
				*parent = *newest_element;
				*newest_element = swap_buffer;
				// 6. With the element in its new location, go back to step 2
			}
		}
	}
}

static void heap_down(heap_t *heap) {
	// Write me!
}

void heap_insert(heap_t *heap, int32_t value) {
	// The new element is always added to the end of a heap
	heap->store[(heap->size)++] = value;
	heap_up(heap);
}

int32_t heap_extract(heap_t *heap) {
	// The root value is extracted, and the space filled by the value from the end
	// If the heap is empty, this will fail horribly...
	int value = heap->store[0];
	heap->store[0] = heap->store[--(heap->size)];
	heap_down(heap);
	return value;
}

uint_fast8_t heap_isEmpty(heap_t *heap) {
	return !(heap->size);
}
