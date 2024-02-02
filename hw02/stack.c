#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "stack.h"

bool is_empty(Stack const* a_stack) {
	bool status;						//Boolean variable to know if stack is empty or not
	if((a_stack -> top) != NULL) {		//If stack is not empty, set status as false
		status = false;
	}
	else {								//If stack is empty, set status as true
		status = true;
	}
	return(status);
}

Stack create_empty_stack(size_t max_block_size_bytes) {
	assert(max_block_size_bytes >= MIN_NODE_BLOCK_SIZE_BYTES);						
	BlockNode val_size;																//Created variable to get the size of the value
	size_t bytes_per_val = sizeof(val_size.values[0]);								//Get size of the value
	size_t array_size = max_block_size_bytes - MIN_NODE_BLOCK_SIZE_BYTES;			//Figure out size of array for values
	size_t node_size = (array_size / bytes_per_val) + 1;							//Calculate number of values node can hold
	Stack created_stack;															//New stack variable created
	created_stack = (Stack){.top = NULL, .num_values_per_node = node_size};			//Set new stack variable equal to wanted values
	return created_stack;
}

void push(Stack* a_stack, int value) {
	//If the stack is empty or top blocknode is full, make an empty blocknode with the correct number of spaces, set values to the values needed, and set the top of the stack to the new node
	if (is_empty(a_stack) || ((a_stack -> top -> num_empty_slots)==0)) {
        BlockNode* new_top = malloc(sizeof((new_top -> values[0])*(a_stack -> num_values_per_node - 1)) + sizeof(*new_top));
        *new_top = (BlockNode){.next = a_stack -> top, .num_empty_slots = a_stack -> num_values_per_node - 1, .values[0] = value};
        a_stack -> top = new_top;
    }
	//If the stack is not empty or top blocknode is not full, set the top of the stack to the value and decrement the empty slots by one
	else {
        a_stack -> top -> values[(a_stack -> num_values_per_node) - (a_stack -> top -> num_empty_slots)] = value;
        (a_stack -> top -> num_empty_slots) -=1 ;
    }
	assert(!is_empty(a_stack));  // Post-condition: stack must not be empty when function exits
}

int pop(Stack* a_stack) {
	assert(!is_empty(a_stack));  // Pre-condition: stack must not be empty when function begins
	int ind_pop = (a_stack -> num_values_per_node) - (a_stack -> top -> num_empty_slots) - 1;	//Obtain index of node that is being popped
	int val_pop = a_stack -> top->values[ind_pop];			//Obtain value of the node that is being popped
	if(ind_pop != 0) {										//If index of node being popped is not the last, increment the count for empty slots
		(a_stack->top->num_empty_slots) += 1;
	}
	//Else, save the value to a temporary variable, set the value of top to the next index value, and free the temporary blocknode
	else {
		BlockNode* top_orig = a_stack -> top;
		a_stack -> top = top_orig -> next;
		free(top_orig);
	}
	return(val_pop);
}

void empty(Stack* a_stack) {
	//While there are values to be emptied, make a temporary node and set its value to the top value, set top to the value of the next index, and free the temporary blocknode
	while(a_stack->top != NULL) {
		BlockNode* temp;
		temp = a_stack -> top;
		a_stack -> top = a_stack -> top -> next;
		free(temp);
	}
	assert(is_empty(a_stack));  // Post-condition: stack must be empty when function exits
	return;
}

int peek_value_from_top_node(Stack const* a_stack) {
	assert(!is_empty(a_stack));  // Pre-condition: stack must not be empty
	//View the top value in the stack from the peek index
	int val_top = a_stack->top->values[((a_stack->num_values_per_node) - (a_stack->top->num_empty_slots) - 1)];
	return(val_top);
}


#define STACK_C_VERSION_2  // Do not delete or modify this line.  It helps us track any updates.
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
