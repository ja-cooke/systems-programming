#include "heap.h"

static void heap_up(heap_t *heap) {
	// 1. Start with the last element in the heap
	
	// offset array to concatinate the 'store' array from 1
	// This makes navigation of the heap more human readable
	int32_t * const offset_store = heap->store - 1;
	uint32_t new_element_index = heap->size;
	uint32_t parent_index = 0;
	
	// 2. If it's the root element, stop
	while(new_element_index != 1){
		// Determine the index of the parent node
		parent_index = new_element_index / 2;	
			
		// 3. Compare it with its parent
		if(offset_store[parent_index] <= offset_store[new_element_index]){
			// 4. If the parent is smaller or equal, stop
			return;
		}
		else{
			// 5. Swap the element with its parent
			// Store the parent value in a memory buffer
			int32_t swap_buffer;
			swap_buffer = offset_store[parent_index];
					
			// Swap the parent and child values
			offset_store[parent_index] = offset_store[new_element_index];
			offset_store[new_element_index] = swap_buffer;
					
			// Change the location of the pointer to the new element
			offset_store[new_element_index] = offset_store[parent_index];
			new_element_index = parent_index;
					
			// 6. With the element in its new location, go back to step 2
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
