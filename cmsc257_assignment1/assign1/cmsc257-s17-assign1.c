////////////////////////////////////////////////////////////////////////////////
//
//  File           : cmsc257-s17-assign1.c
//  Description    : This is the main source code for for the first assignment
//                   of CMSC257.  See the related
//                   assignment page for details.
//
//   Author        : Nathan West
//   Last Modified : 10/11/17
//

// Include Files
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Project Includes
#include <cmsc257-s17-assign1-support.h>
//
// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : The main function for the CMSC257 assignment #1
//
// Inputs       : argc - the number of command line parameters
//                argv - the parameters
// Outputs      : 0 if successful test, -1 if failure

int main(int argc, char *argv[]) {

	// Local variables
	// NOT E: this is where you will want to add some new variables
	float f_array[20];
    int i, size = 20; // m_array[25];
    int int_array[20];

	// Step #1 - read in the float numbers to process
	for (i = 0; i < size; i++) {
		scanf("%f", &f_array[i]);
	}

    // Step #2 - convert the floating point values to integers
    for (i = 0; i < size; i++) {
        int_array[i] = (int) f_array[i]; // conversion to integer
        int_array[i] = abs(int_array[i]); // absolute value
        int_array[i] = int_array[i] & 15; // mod by 16
    }
    printf("\n");

    // Step #3 - print out the integer and floating point arrays
    float_display_array(f_array, size); printf("\n");
    integer_display_array(int_array, size); printf("\n");

    // Step #4 - print out the number of '1's in each integer
    printf("\n");
    for (i = 0; i < size; i++) {
        printf("%d's bits = %d\n", int_array[i], countBits(int_array[i]));
    }
    printf("\n");

    // Step #5 - Sort the integer array using QuickSort and print the sorted array
    printf("Unsorted integer array\n");
    for (i = 0; i < size; i++) {
        printf("%d ", int_array[i]);
    }
    printf("\n");

    integerQuickSort(int_array, 0, 19);
    printf("Sorted integer array\n");
    for (i = 0; i < size; i++) {
        printf("%d ", int_array[i]);
    }
    printf("\n");

    // Step #6 - figure out number of evens in each array (float and sorted integer arrays)
    printf("\nEven numbers in float array = %d\n", float_evens(f_array, size));
    printf("Even numbers in integer array = %d\n", integer_evens(int_array, size));
    printf("\n");


    // Step #7 - print the most frequent values from the sorted integer array
    printf("Most value = %d\n", most_values(int_array, size));
    printf("\n");

    // Step #8 - print the integers corresponding to the reverse bit string from integer array
    printf("Binary string (Represented in 4 bits)\n");
    for(i = 0; i < size; i++) {
        printf("%d = ", int_array[i]);
        binaryString(int_array[i]);
        printf("\n");
    }
    printf("\n");


    printf("Reverse binary string (Represented in 4 bits)\n");
    for (i = 0; i < size; i++) {
        printf(" = %d\n",reverseBits((unsigned short)int_array[i]));
    }

    // Return successfully
	return(0);
}
