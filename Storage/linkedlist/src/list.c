#include "list.h"

void list_insert(list_t *list, list_item_t *item) {
	// Check to see if the list is empty. If it is, the new item will need to link to
	//itself (in both directions) and become the new list head.
	if(list->head == 0){
		list->head = item;
		item->next = item;
		item->prev = item;
	}
	//– Insert the new item by updating the pointers in the item and pointers in
	//the two existing items that will be either side of it.
	else{
		// Initialise pointers for the two list items to be moved
		list_item_t *previous_item;
		list_item_t *next_item;
		
		// Point the pointers to these items
		next_item = list->head;
		previous_item = list->head->prev;
		
		// Update the list pointers
		previous_item->next = item;
		item->prev = previous_item;
		item->next = next_item;
		next_item->prev = item;
	}
}

void list_remove(list_t *list, list_item_t *item) {
	// – Check to see if the item being removed is the only one in the list. If it is,
	// you’ll need to set the head pointer to zero.
	if(item->next == item){
		list->head = 0;
	}
	//– Remove the item by updating the pointers items either side of it to point
	// to each other.
	else{
		list_item_t *next_item;
		list_item_t *previous_item;
		
		next_item = item->next;
		previous_item = item->prev;
		
		next_item->prev = previous_item;
		previous_item->next = next_item;
		//– Update the head pointer if the item being removed was the head of the
		//list.
		if(item == list->head){
			list->head = next_item;
		}
	}
}

list_item_t * list_next(list_t *list) {
	// return an item from the list, and update the head pointer to
	// point to the next one.
	list_item_t *output_item = list->head;
	
	list->head = list->head->next;
	
	return output_item;
}
