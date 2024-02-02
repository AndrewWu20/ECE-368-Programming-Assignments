#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "btree.h"

void convert_ascending(BTreeNode* current, int* result, int* index, size_t num_elements); //Helper function to convert arrays into ascending order

// Instructor solution add/removes about 25 lines in this file.

int query_min(BTreeNode* root) {
	// Return the smallest (minimum value) key in the b-tree.
	//
	// Rules:  (1) No recursion.  (2) no malloc(…)  (3) Θ(h)
	BTreeNode* current = root;
	while(get_subtree(current, 0) != NULL) {									//While there are lower subtrees, move to the current root's first subtree
		current = get_subtree(current, 0);
	}
	int val_min = current -> keys[0];											//The first key's minimum value is set to the variable which is then returned
	return val_min;
}

int query_max(BTreeNode* root) {
	// Return the largest (maximum value) key in the b-tree.
	//
	// Rules:  (1) No recursion.  (2) no malloc(…)  (3) Θ(h)
	BTreeNode* current = root;
	while(get_subtree(current, current -> num_keys) != NULL) {					//While there are lower subtrees, move to current root's last subtree
		current = get_subtree(current, current -> num_keys);
	}
	int val_max = current -> keys[current -> num_keys - 1];						//The last key's maximum value is set to the variable which is then returned
	return val_max;
}

KeyQueryResult query_k_largest(BTreeNode* root, size_t k) {
	// Return an array of the (up to) k largest keys in the b-tree.

	// Rules:  (1) No recursion.  (2) no malloc(…)  (3) Θ(h)

	int* k_largest_keys = malloc(k * sizeof *k_largest_keys);
	int num_keys_found = 0;

	// TODO: Delete this line and complete this function.
	BTreeNode* current = root;
	int* key_array = malloc(count_keys(root) * sizeof *key_array);				//Array that contains all keys in the subtree
	int index_key = 0;															//Variable to track array index
	convert_ascending(current, key_array, &index_key, count_keys(root));		//Convert array into ascending order
	for(int i = 0; i < k; i++) {												//Create and fill array with the largest keys in subtree
		k_largest_keys[i] = key_array[count_keys(root) - 1 - i];
		num_keys_found++;
	}
	free(key_array);															//Free from malloc
	return (KeyQueryResult) { .keys = k_largest_keys, .num_keys = num_keys_found };
}

KeyQueryResult query_k_smallest(BTreeNode* root, size_t k) {
	// Return an array of the (up to) k smallest keys in the b-tree.

	int* k_smallest_keys = malloc(k * sizeof *k_smallest_keys);
	int num_keys_found = 0;

	// TODO: Delete this line and complete this function.
	BTreeNode* current = root;
	int* key_array = malloc(count_keys(root) * sizeof *key_array);				//Array that contains all keys in the subtree
	int index_key = 0;															//Variable to track array index
	convert_ascending(current, key_array, &index_key, count_keys(root));		//Convert array into ascending order
	for(int i = 0; i < k; i++) {												//Create and fill array with the smallest keys in subtree
		k_smallest_keys[i] = key_array[i];
		num_keys_found++;
	}
	free(key_array);															//Free from malloc
	return (KeyQueryResult) { .keys = k_smallest_keys, .num_keys = num_keys_found };
}

//Helper function uses recusion on the subtrees of the tree and add each key to an array
void convert_ascending(BTreeNode* current, int* result, int* index, size_t num_elements) {
	if((current != NULL) && (*index < num_elements)) {										//Checks for valid subtree
		convert_ascending(get_subtree(current,0), result, index, num_elements);				//Use recursion on first subtree
		for(int i = 0; i < current -> num_keys; i++) {										//For each key, add key to array and increment index
			result[*index] = current -> keys[i];	
			(*index)++;	
			convert_ascending(get_subtree(current, i +1), result, index, num_elements);		//Use recursion on remaining subtrees
		}
	}
}
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
