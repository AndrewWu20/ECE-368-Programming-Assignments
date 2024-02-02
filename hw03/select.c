#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "select.h"

void swap(int* a_n1, int* a_n2) {		//Function assigns a_n2 value to a temp variable, sets value of a_n2 to a_n1, then assigns value of a_n1 to temp variable
	int temp_val = *a_n2;
	*a_n2 = *a_n1;
	*a_n1 = temp_val; 
}

bool is_partition_correct(int const* numbers, size_t num_numbers, size_t new_numbers_0) {
	int idx_pivot_val = new_numbers_0 - 1;					//Get pivot value index 
	for(int i = 0; i < idx_pivot_val; i++) {				//Check that all values in the lower partitions are less than pivot value
		if(numbers[idx_pivot_val] < numbers[i]) {
			return false;									//If not, return false
		}
	}
	for(int n= idx_pivot_val + 1; n < num_numbers; n++) {	//Check that all values in the upper partition are greater than pivot value
		if(numbers[idx_pivot_val] >= numbers[n]) {
			return false;									//If not, return false
		}
	}
	return true;
}

size_t partition(int* numbers, size_t num_numbers) {
	size_t num_left_elements = 0;								//Set number of values in left partition to zero
	for(int i = 0; i < num_numbers - 1; i++) {					//Check every element to see if it belongs in the left partition
		if(numbers[num_numbers - 1] >= numbers[i]) {			//If the element belongs in the left partition, assign it to the left partition and increase the number of values in left partition
			num_left_elements++;
			swap(&numbers[num_left_elements - 1], &numbers[i]);
		}
	}
	swap(&numbers[num_left_elements++], &numbers[num_numbers - 1]);		//Increase number of values in left partition for pivot and move pivot to left partition
	return(num_left_elements);
}

int get_kth_smallest(size_t k_0_based, int* numbers, size_t num_numbers) {
	assert(k_0_based < num_numbers);								//Ensure k_0_based is within range
	size_t idx_pivot_val = partition(numbers, num_numbers) - 1;		//Find pivot value to use when partitioning
	if(num_numbers == 1) {											//Return value as kth smallest if there is one value in the array
		return(numbers[0]);
	}
	if(idx_pivot_val > k_0_based) {									//If wanted element is in the left partition, use recursion to find and return it
		return get_kth_smallest(k_0_based, numbers, idx_pivot_val);
	}
	else if (idx_pivot_val == k_0_based) {							//If wanted element is the pivot element, return it
        return(numbers[idx_pivot_val]);
    }
    else {															//If wanted element is in the right partition, use recursion to find and return it
		size_t right_part_k_0_based = k_0_based - idx_pivot_val - 1;
		int* right_part_numbers = numbers + idx_pivot_val + 1;
		size_t right_part_num_numbers = num_numbers - idx_pivot_val - 1;
        return get_kth_smallest(right_part_k_0_based, right_part_numbers, right_part_num_numbers);
	}
}


#define SELECT_C_V2  // Do not delete or modify this line.  It helps us track any updates.
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
