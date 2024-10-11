#include "heap.h"

static void swap_elements(int32_t *element_a, int32_t *element_b){
	// Swaps the contents of two memory elements
	// Store the value of element_a in a memory buffer
	int32_t swap_buffer;
	swap_buffer = *element_a;
					
	// Swap the values in memory addressed by the pointers
	*element_a = *element_b;
	*element_b = swap_buffer;
	
	// Store the memory address of element_a in a buffer
	int32_t * swap_buffer_ptr;
	swap_buffer_ptr = element_a;
	
	// Swap the memory addresses stored in the pointers
	element_a = element_b;
	element_b = swap_buffer_ptr;
}

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
			// Obtain pointers to the parent and child elements
			int32_t *parent = &offset_store[parent_index];
			int32_t *child = &offset_store[new_element_index];
			
			// Swap the two elements
			swap(parent, child);
					
			// Change the location of the pointer to the new element
			new_element_index = parent_index;
					
			// 6. With the element in its new location, go back to step 2
		}
	}
}

static void heap_down(heap_t *heap) {
	// offset array to concatinate the 'store' array from 1
	// This makes navigation of the heap more human readable
	int32_t * const offset_store = heap->store - 1;
	uint32_t parent_index;
	uint32_t child_index_a = (uint32_t) -1;
	uint32_t child_index_b = (uint32_t) -1;
	
	// 1. Start with the root element
	parent_index = 1;
	
	// 2. If it has no children, stop
	while((parent_index * 2) <= heap->size){
		// 3. Compare with its children
		
		// Determine the indices of the child nodes
		child_index_a = (2*parent_index);
		child_index_b = (2*parent_index)+1;
		
		// Detemine which of the two children has a smaller value
		uint32_t smallest_child = (
											offset_store[child_index_a] < offset_store[child_index_b]
											) 
											? child_index_a : child_index_b;
			
		if(offset_store[parent_index] <= offset_store[smallest_child]){
			// 4. If the element is smaller than or equal to both children, stop
			return;
		}
		else{
			
			
			// 5. Swap the element with the smaller of the children
			// Obtain pointers to the parent and child elements
			int32_t *parent = &offset_store[parent_index];
			int32_t *smallest_child_ptr = &offset_store[smallest_child];
			
			// Swap the two elements
			swap(parent, smallest_child_ptr);
					
			// Change the location of the pointer to the new element
			parent_index = smallest_child;
			// 6. With the element in its new location, go back to step 2
		}
	}
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
