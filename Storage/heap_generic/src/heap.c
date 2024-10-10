#include "heap.h"

static void heap_up(heap_t *heap) {
	// 1. Start with the last element in the heap
	
	// offset array to concatinate the 'store' array from 1
	// This makes navigation of the heap more human readable
	void ** const offset_store = heap->store - 1;
	uint32_t new_element_index = heap->size;
	uint32_t parent_index = 0;

	// 2. If it's the root element, stop
	while(new_element_index != 1){
		// Determine the index of the parent node
		parent_index = new_element_index / 2;	

		// Call the comparator function
		void * parent_pointer = offset_store[parent_index];
		void * new_element_pointer = offset_store[new_element_index];
		
		// 3. Compare it with its parent
		if(heap->comparator(parent_pointer, new_element_pointer) <= 0){
			// 4. If the parent is smaller or equal, stop
			return;
		}
		else{
			// 5. Swap the element with its parent
			// Store the parent value in a memory buffer
			void * swap_buffer_ptr;
			swap_buffer_ptr = parent_pointer;
					
			// Swap the parent and child values
			parent_pointer = new_element_pointer;
			new_element_pointer = swap_buffer_ptr;
					
			// Change the location of the pointer to the new element
			new_element_index = parent_index;
					
			// 6. With the element in its new location, go back to step 2
		}
	}
}

static void heap_down(heap_t *heap) {
	// offset array to concatinate the 'store' array from 1
	// This makes navigation of the heap more human readable
	void ** const offset_store = heap->store - 1;
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
		
		void * parent_pointer = offset_store[parent_index];
		void * child_a_pointer = offset_store[child_index_a];
		void * child_b_pointer = offset_store[child_index_b];
		
		// Detemine which of the two children has a smaller value
		void * smallest_child;
		uint32_t smallest_child_index;
		if (heap->comparator(child_a_pointer, child_b_pointer) <= 0){
			smallest_child = child_a_pointer;
			smallest_child_index = child_index_a;
		}
		else{
			smallest_child = child_b_pointer;
		}
			
		if(heap->comparator(parent_pointer, smallest_child) <= 0){
			// 4. If the element is smaller than or equal to both children, stop
			return;
		}
		else{
			// 5. Swap the element with the smaller of the children
			// Store the parent value in a memory buffer
			void * swap_buffer_ptr;
			swap_buffer_ptr = parent_pointer;
			
			// Swap the parent and child values
			parent_pointer = smallest_child;
			smallest_child = swap_buffer_ptr;
					
			// Change the location of the pointer to the new element
			parent_index = smallest_child_index;
			// 6. With the element in its new location, go back to step 2
		}
	
	}
}

void heap_insert(heap_t *heap, void * item) {
	// The new element is always added to the end of a heap
	heap->store[(heap->size)++] = item;
	heap_up(heap);
}

void * heap_extract(heap_t *heap) {
	// The root value is extracted, and the space filled by the value from the end
	// If the heap is empty, this will fail horribly...
	void * item = heap->store[0];
	heap->store[0] = heap->store[--(heap->size)];
	heap_down(heap);
	return item;
}

uint_fast8_t heap_isEmpty(heap_t *heap) {
	return !(heap->size);
}
