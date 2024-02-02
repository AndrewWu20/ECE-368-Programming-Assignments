#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "btree.h"

/********************************************************************
|* RULES
|*
|*  1. Do not access 'node -> subtrees' directly.  Use wrappers (accessor functions) instead.
|*   ┌──────────────────────────┬────────────────────────────────────────────┐
|*   │ Instead of this (▼),     │ Use this (▼).                              │
|*   ╞══════════════════════════╪════════════════════════════════════════════╡
|*   │ node -> subtrees[…]      │ get_subtree(node, subtree_idx)             │
|*   ├──────────────────────────┼────────────────────────────────────────────┤
|*   │ node -> subtrees[…] = …; │ set_subtree(node, subtree_idx, new_subtree)│
|*   ├──────────────────────────┼────────────────────────────────────────────┤
|*   │ &(node -> subtrees[…])   │ get_address_of_subtree(node, subtree_idx   │
|*   └──────────────────────────┴────────────────────────────────────────────┘
|*      ∘ When testing your code, we may modify the wrappers to track which nodes were accessed.
|*      ∘ This applies only to 'node -> subtrees'.  You may access all other fields like usual.
|*
|*  2. Use descriptive names that accurately indicate the variable's type and contents.
|*      ∘ Use 'key_idx' (or 'keyIdx') or 'subtree_idx' (or 'subtreeIdx'), not 'idx', 'i', or 'pos'.
|*      ∘ Use 'copy_of_▒' (or 'copyOf▒') instead of 'temp' if you need to swap two values.
|*      ∘ See code quality standards for other naming conventions.
|*
|*  3. Comment any code that is not self-explanatory.
|*      ∘ See btree_insert.c for examples.
|*
|*  4. Use assertions to document and sanity-check values.
|*      ∘ Ex: Check that indexes are in the expected range.
|*      ∘ Some assertions have already been added to the stub functions below.
|*      ∘ You must add more, as appropriate.  (There is not specific number.)  This is to help you.
|*
|* This is in addition to the requirements in the assignment page.
|*
|* WARNING: There may be penalties for submissions that do not follow these rules.
\*******************************************************************/



bool delete_key(BTreeNode** a_root, int key) {
	// Determine if the key is in a leaf or not
	// Find it on the b-tree, traverse down the tree until we find the key
	int idx = find_key(a_root, key);
	if(is_key_present(a_root -> keys, a_root -> num_keys, key)) {
		if(get_subtree(a_root, 0) == NULL) {
			remove_leaf(a_root, idx);
		}
		else {
			remove_non_leaf(a_root, idx);
		}
	}
	else {
		if(get_subtree(a_root, 0) == NULL) {
			printf("The key %d does not exist in the tree\n", key);
            return;
		}
		bool flag = ((idx == a_root -> keys) ? true : false);
		int subtree_idx = 0;
        while (subtree_idx < a_root->num_keys && a_root->keys[subtree_idx] < key) {
            subtree_idx++;
        }
		if(get_subtree(a_root, subtree_idx) -> num_keys < MIN_SUBKEYS) {
			fill_subtree(*a_root, subtree_idx);
		}
		if((flag && subtree_idx) > (a_root -> num_keys)) {
			delete_key(get_subtree(a_root, subtree_idx - 1), key);
		}
		else{
			delete_key(get_subtree(a_root, subtree_idx), key);
		}
	}
	return false;
}

bool is_key_present(int* node, int node_size, int key) {
    for (int i = 0; i < node_size; i++)
    {
        if (node[i] == key)
        {
            return true;
        }
    }
    return false;
}

void remove_leaf(BTreeNode** a_root, int index) {
	for(int i  = index + 1; i < a_root -> num_keys; i++) {
		a_root -> keys[i - 1] = a_root -> keys[i];
	}
	a_root -> keys--;
	return;
}

void remove_non_leaf(BTreeNode** a_root, int index) {
	int key = a_root -> keys[index];
	if(get_subtree(a_root, index) -> num_keys >= MIN_SUBKEYS) {
		KeyLocation location = (KeyLocation) {.key = key, .node = a_root, .key_idx = index};
		int predecessor = get_in_order_predecessor(location);
		a_root -> keys[index] = predecessor;
		remove_non_leaf(get_subtree(a_root, index), predecessor);
	}
	else if(get_subtree(a_root, index + 1) -> num_keys >= MIN_SUBKEYS) {
		KeyLocation location = (KeyLocation) {.key = key, .node = a_root, .key_idx = index};
		int successor = get_in_order_successor(location);
		a_root -> keys[index] = successor;
		remove_non_leaf(get_subtree(a_root, index + 1), successor);
	}
	else{
		merge_subtree_with_next(a_root, index);								//Not sure if correct
		remove_non_leaf(get_subtree(a_root, index), a_root -> key);			//Not sure if correct
	}
}

void fill_subtree(BTreeNode* node, int idx) {
	if(idx != 0 && get_subtree(a_root, index + 1) -> num_keys >= MIN_SUBKEYS) {
		borrow_prev(node, idx);
	}
	else if(idx != get_address_of_subtree(node, index) && get_subtree(a_root, index + 1) -> num_keys >= MIN_SUBKEYS) {
		borrow_next(node, idx);
	}
	else{
		if(idx != get_address_of_subtree(node, index)) {
			merge_subtree_with_next(node, idx);
		}
		else{
			merge_subtree_with_next(node, idx - 1);
		}
	}
	return;
}

void borrow_prev(BTreeNode* node, int idx) {
	BTreeNode* child = get_subtree(node, idx);
	BTreeNode* siblig = get_subtree(node, idx - 1);
	for(int i = child -> num_keys - 1; i >= 0; i--) {
		child -> keys[i + 1] = child -> keys[i];
	}
	if(!child -> get_subtree(node, 0) == NULL) {
		for(int i = child -> num_keys; i >= 0; i--) {
			get_subtree(child, idx + 1) = get_subtree(child, idx);
		}
	}
	child -> keys[0] = node -> keys[idx - 1];
	if(!child -> get_subtree(node, 0) == NULL) {
		get_subtree(child, 0) = get_subtree(sibling, sibling -> num_keys);
	}
	node -> keys[idx - 1] = sibling -> keys[sibling -> num_keys - 1];
	child -> num_keys += 1;
	sibling -> num_keys -= 1;
	return;
}

void borrow_next(BTreeNode* node, int idx) {
	BTreeNode* child = get_subtree(node, idx);
	BTreeNode* siblig = get_subtree(node, idx + 1);
	child -> keys[child -> num_keys] = node -> keys[idx];
	if(!child -> get_subtree(node, 0) == NULL) {
		get_subtree(child, (child -> num_keys) + 1) = get_subtree(sibling, 0);
	}
	node -> keys[idx] = sibling -> keys[0];
	for(int i = 1; i < sibling -> num_keys; i++) {
		sibling -> keys[idx - 1] = sibling -> keys [idx];
	}
	if(!sibling -> get_subtree(node, 0) == NULL) {
		for(int i = 1; i <= sibling -> num_keys; i++) {
			get_subtree(sibling, idx - 1) = get_subtree(sibling, idx);
		}
	}
	child -> num_keys += 1;
	sibling -> num_keys -= 1;
	return;
}



void merge_subtree_with_next(BTreeNode* parent, int subtree_1_idx) {
	int subtree_2_idx = subtree_1_idx + 1;
	assert(subtree_1_idx >= 0 && subtree_1_idx <= parent -> num_keys);
	assert(subtree_2_idx >= 0 && subtree_2_idx <= parent -> num_keys);

	// Get the children to be merged.
	BTreeNode* subtree_1 = get_subtree(parent, subtree_1_idx);
	BTreeNode* subtree_2 = get_subtree(parent, subtree_1_idx + 1);
	assert(subtree_1 -> num_keys + subtree_2 -> num_keys <= MAX_KEYS);	// Assert: the two subtrees have enough space to be merged

	int used_keys = subtree_1 -> num_keys;											//Initialize number of keys used in subtree_1
	subtree_1 -> keys[used_keys] = parent -> keys[subtree_1_idx];					//Move key from parent to first subtree
	subtree_1 -> num_keys += subtree_2 -> num_keys;									//Add space for keys from subtree_1 in subtree_1
	for (int i = used_keys; i < MAX_KEYS - 1; i++) {
		subtree_1 -> keys[i + 1] = subtree_2 -> keys[i - used_keys];				//Move keys from second to first subtree
	}
	parent -> keys[subtree_1_idx] = subtree_2 -> keys[subtree_2 -> num_keys - 1];	//Move key from second subtree to the parent
}

void shift_key_forward(BTreeNode* parent, int src_subtree_idx) {
	// Move the last key from get_subtree(parent, src_subtree_idx) to the next subtree.
	int dst_subtree_idx = src_subtree_idx + 1;
	assert(src_subtree_idx >= 0 && src_subtree_idx <= parent -> num_keys);
	assert(dst_subtree_idx >= 1 && dst_subtree_idx <= parent -> num_keys);

	BTreeNode* src_subtree = get_subtree(parent, src_subtree_idx);
	BTreeNode* dst_subtree = get_subtree(parent, dst_subtree_idx);
	// assert(src_subtree -> num_keys > MAX_KEYS);      // Assert: source has ≥1 extra node
	// assert(dst_subtree -> num_keys == MAX_KEYS - 1); // Assert: destination has too few

	dst_subtree -> num_keys++;															//Increase number of keys in destination subtree
	for (int i = dst_subtree -> num_keys; i > 0; i--) {									//Shift keys up individually to source subtree to make room for new key
		dst_subtree -> keys[i] = dst_subtree -> keys[i - 1];	
	}
	dst_subtree -> keys[0] = parent -> keys[src_subtree_idx];							//Move the key from the parent to the destination subtree
	parent -> keys[src_subtree_idx] = src_subtree -> keys[src_subtree -> num_keys - 1];	//Move key from source to parent subtree
	src_subtree -> num_keys--;															//Decrease number of keys in source subtree
}

void shift_key_backward(BTreeNode* parent, int src_subtree_idx) {
	// Move the first key from get_subtree(parent, src_subtree_idx) to previous subtree.
	int dst_subtree_idx = src_subtree_idx - 1;
	assert(src_subtree_idx >= 1 && src_subtree_idx <= parent -> num_keys);
	assert(dst_subtree_idx >= 0 && dst_subtree_idx <= parent -> num_keys);

	BTreeNode* src_subtree = get_subtree(parent, src_subtree_idx);
	BTreeNode* dst_subtree = get_subtree(parent, dst_subtree_idx);
	// assert(src_subtree -> num_keys > MAX_KEYS);
	// assert(dst_subtree -> num_keys == MAX_KEYS - 1);

	dst_subtree -> num_keys++;															//Increase number of keys in destination subtree
	dst_subtree -> keys[dst_subtree -> num_keys] = parent -> keys[src_subtree_idx - 1];	//Move key from parent to destination subtree
	parent -> keys[src_subtree_idx - 1] = src_subtree -> keys[0];						//Move key from source to parent subtree
	for (int i = 0; i < src_subtree -> num_keys - 1; i++) {								//For each key, shift them down into subtree individually
		src_subtree -> keys[i] = src_subtree -> keys[i + 1];							
	}
	src_subtree -> num_keys--;															//Decrease number of keys in source subtree
}

KeyLocation get_in_order_predecessor(KeyLocation key_location) {
	// Return the key (with location) for key that would precede this one in an in-order traversal.

	assert(get_subtree(key_location.node, 0) != NULL);  		//Assert that current location is not a leaf
	BTreeNode* current = key_location.node;						//Current node
	current = get_subtree(current, key_location.key_idx);		//Move to the subtree to the left of the key
	while(get_subtree(current, current -> num_keys) != NULL) {	//While the subtree is not null, move to the right until we hit a leaf
		current = get_subtree(current, current -> num_keys);
	}
	return (KeyLocation) { .key = current -> keys[current -> num_keys - 1], .node = current, .key_idx = current -> num_keys - 1 };
}

KeyLocation get_in_order_successor(KeyLocation key_location) {
	// Return the key (with location) for key that would follow this one in an in-order traversal.

	assert(get_subtree(key_location.node, 0) != NULL);  		//Assert that current location is not a leaf
	BTreeNode* current = key_location.node;						//Current node
	current = get_subtree(current, key_location.key_idx + 1);	//Move to the subtree to the right of the key
	while(get_subtree(current, 0) != NULL) {					//While the subtree is not null, move to the left until we hit a leaf
		current = get_subtree(current, 0);
	}
	return (KeyLocation) { .key = current -> keys[0], .node = current, .key_idx = 0 };
}
