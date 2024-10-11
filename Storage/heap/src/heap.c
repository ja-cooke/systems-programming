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

static int_fast8_t comparator(int32_t * element_a, int32_t * element_b) {
	if(*element_a < *element_b){
		return 1;
	}
	else if(*element_a > *element_b){
		return -1;
	}
	else{
		return 0;
	}
}

static uint32_t find_children(uint32_t parent_index, uint32_t heap_size){
	// Determines the number of child nodes for a parent node of index 'parent_index' in a 
	// binary heap of size 'heap_size'.
	uint32_t index_of_first_child = parent_index*2;
	
	if(index_of_first_child > heap_size){
		return 0;
	}
	else if (index_of_first_child == heap_size){
		return 1;
	}
	else{
		return 2;
	}
}

static void heap_up(heap_t *heap) {
	// offset array to concatinate the 'store' array from 1
	// This makes navigation of the heap more human readable
	int32_t * const offset_store = heap->store - 1;
	// 1. Start with the last element in the heap
	uint32_t new_element_index = heap->size;
	// Initialise the parent_index as 0. This will be updated.
	uint32_t parent_index = 0;
	
	int32_t *parent = 0;
	int32_t *child = 0;
	
	// 2. If it's the root element, stop
	while(new_element_index != 1){
		// Determine the index of the parent node.
		// For a binary heap, indices of children for a parent with index n are 2n & 2n+1.
		// Hence the parent of a child can be found using integer division by 2.
		parent_index = new_element_index / 2;	
			
		// Direct pointers to the parent and child elements
		parent = &offset_store[parent_index];
		child = &offset_store[new_element_index];
		
		// 3. Compare it with its parent
		if(comparator(parent, child) >= 0){
			// 4. If the parent is smaller or equal, stop
			return;
		}
		else{
			// 5. Swap the element with its parent
			swap_elements(parent, child);
					
			// Update the value of the index to the new element
			new_element_index = parent_index;
					
			// 6. With the element in its new location, go back to step 2
		}
	}
}

static void heap_down(heap_t *heap) {
	// offset array to concatinate the 'store' array from 1
	// This makes navigation of the heap more human readable
	int32_t * const offset_store = heap->store - 1;
	
	// Initialise all variables and pointers to zero
	int32_t *parent = 0;
	int32_t *child_a = 0;
	int32_t *child_b = 0;
	int32_t *smallest_child = 0;
	
	uint32_t number_of_children;
	
	uint32_t parent_index = 0;
	uint32_t child_index_a = 0;
	uint32_t child_index_b = 0;
	uint32_t smallest_child_index = 0;
	
	int_fast8_t comparator_result = 0;
	
	// 1. Start with the root element as a 'parent node'
	parent_index = 1;
	parent = &offset_store[parent_index];
	
	number_of_children = find_children(parent_index, heap->size);
	
	// 2. If it has no children, stop
	// The parent will have no children if it's index * 2 is greater than the size of the heap.
	// (i.e. the index of it's first child would be beyond the last heap element.)
	while(number_of_children > 0){
		// 3. Compare with its children
		// Case for only one child
		if(number_of_children == 1){
			smallest_child_index = parent_index*2;
			smallest_child = &offset_store[smallest_child_index];
		}
		// Case for two children
		else{
			// For a binary heap, indices of children for a parent with index n are 2n & 2n+1.
			child_index_a = (2*parent_index);
			child_index_b = (2*parent_index)+1;
			
			child_a = &offset_store[child_index_a];
			child_b = &offset_store[child_index_b];
			
			// Detemine which of the two children has a smaller value
			comparator_result = comparator(child_a, child_b); 
			
			if(comparator_result >= 0){
				smallest_child = child_a;
				smallest_child_index = child_index_a;
			}
			else{
				smallest_child = child_b;
				smallest_child_index = child_index_b;
			}
		}
		
		// Now the smallest child has been determined
		
		if(comparator(parent, smallest_child) >= 0){
			// 4. If the element is smaller than or equal to both children, stop
			return;
		}
		else{
			// 5. Swap the element with the smaller of the children
			// Swap the two elements
			swap_elements(parent, smallest_child);
					
			// Change the location of the pointer to the new element
			parent_index = smallest_child_index;
			parent = &offset_store[parent_index];
			// recalculate the number of children of the new parent node.
			number_of_children = find_children(parent_index, heap->size);
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
