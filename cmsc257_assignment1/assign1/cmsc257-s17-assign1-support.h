#ifndef CMSC257_A1SUPPORT_INCLUDED
#define CMSC257_A1SUPPORT_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//
//  File          : cmsc257-s17-assign1-support.h
//  Description   : This is a set of general-purpose utility functions we use
//                  for the 257 assignment #1.
//

//
// Functional Prototypes

int float_display_array(float *f_array, int size);
	// This function prints out the array of floating point values

int integer_display_array(int *int_array, int size);
	// This function prints out the array of integer values

int float_evens(float *f_array, int size);
	// Return the number of even numbers in the array (float version)

int integer_evens(int *int_array, int size);
	// Return the number of even numbers in the array (int version)

int countBits(unsigned int number);
	// Count the number of '1' bits in the binary representation

int most_values(int *array, int size);
	// Print out the values with the most occurences in array

unsigned short reverseBits(unsigned short num);
        // Reverse the binary bit string

void binaryString(int n);
        // Print the binary equivalent of the number passed

void integerQuickSort(int int_array[], int low, int high);
        // Sort the integer array using QuickSort algorithm


#endif // CMSC257_A1SUPPORT_INCLUDED
