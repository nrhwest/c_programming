////////////////////////////////////////////////////////////////////////////////
//
//  File           : cmsc257-s17-assign1-support.c
//  Description    : This is the support source code for the main source code
//                   for the first assignment
//                   of CMSC257.  See the related
//                   assignment page for details.
//
//   Author        : Nathan West
//   Last Modified : 10/11/17

// Include Files
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


/*************************************************
  Function: float_display_array
  Description: Prints out the float array

  Input   :f_array – Array with float elements
          :size – size of the float array
  Output  :0, if test successful, -1 if not
*/
int float_display_array(float *f_array, int size) {
  int i;
  printf("\nFloat array\n");
  for (i = 0; i < size; i++) {
    printf("%0.2f ", f_array[i]); // loop through array and print elements
  }
  return 0;
}

/*************************************************
  Function: integer_display_array
  Description: Prints out the integer array

  Input   :int_array – Array with integer elements
          :size – size of the integer array
  Output  :0, if test successful, -1 if not
*/
int integer_display_array(int *int_array, int size) {
  int i;
  printf("\nInteger array\n");
  for (i = 0; i < size; i++) {
    printf("%d ", int_array[i]); // loop through array and print elements
  }
  return 0;
}

/*************************************************
  Function: float_evens
  Description: Counts the number of even floats in the float array

  Input   :f_array – Array with float elements
          :size – size of the float array
  Output  :0, if test successful, -1 if not
*/
int float_evens(float *f_array, int size) {
  int i, count = 0;
  for (i = 0; i < size; i++) {
    if ((int)f_array[i] & 1) { // casts float as int to take bit-wise AND to determine if number in array is even
      count += 1;
    }
   }
  return count;
}

/*************************************************
  Function: integer_evens
  Description: Counts the number of even integers in the integer array

  Input   : int_array – Array with float elements
            size – size of the float array
  Output  : count, if test successful
*/
int integer_evens(int *int_array, int size) {
  int i, count = 0;
  for (i = 0; i < size; i++) {
    if (int_array[i] & 1) { // bit-wise AND to determine if number in array is even
      count += 1;
    }
  }
  return count;
}

/*************************************************
  Function: countBits
  Description: Counts the number of bits in each
               binary representation of the integer

  Input   : An unsigned integer value

  Output  : count, if test successful, -1 if not
*/
int countBits(unsigned int number) {
  unsigned int count = 0;
  while (number) {
    count += number & 1; // increase count if bit-wise AND operator is true
    number >>= 1; // shift right in binary digit to continue counting
  }
  return count;
}

/*************************************************
  Function: most_values
  Description: Counts frequency of each integer in the array

  Input   : int_array – Array with integer elements
            size – size of the integer array
  Output  : majorityElement, depending on number of occurences in the array
*/
int most_values(int *int_array, int size) {
  int i, majorityElement = int_array[0]; // set majorityElement to first element
  int mCnt = 1, currentCnt = 1;
  for (i = 1; i <= size; i++) {
    if (int_array[i - 1] == int_array[i]) { // checks in neighbor elements are same
      currentCnt++;
    } else {
      if (currentCnt > mCnt) {
        mCnt = currentCnt; // sets majority count to current count
        majorityElement = int_array[i - 1];
      }
      currentCnt = 1;
    }
  }
  printf("Most value count = %d, ", mCnt);
  return majorityElement;
}

/*************************************************
  Function: reverseBits
  Description: Reverses binary representation of the integer

  Input   : unsigned short val

  Output  : result, which is the integer form of the reverse binary representation
*/
unsigned short reverseBits(unsigned short val) {
  unsigned short i;
  unsigned short rev = 0;
  unsigned short result = 0;
  unsigned short size = sizeof(val) << 3;

  for (i = 1; i < 16; i = i * 2) {
    if (val & i) { // check if AND operator of val and i is true
      printf("1");
      rev++;
    } else { printf("0"); }
    rev <<= 1; // shift bit values left
  }
  rev >>= 1; // shift bit values right

  for (i = 0; i < size; i++) {
    if ((val >> i) & 1) {
      result |= (1 << (size - 1 - i)); // result equals the OR of the shifting of bit values
    }
  }
  return result;
}

/*************************************************
  Function: binaryString
  Description: Converts an integer to a binary number

  Input   : int_array – Array with integer elements

  Output  : void
*/
void binaryString(int n) {
  int i, bit, binary;
  for (i = 3; i >= 0; i--) {
      binary = 1 << i;
      bit = n & binary; // bit-wise AND of number from array
      if (bit == 0) {
        printf("0");
      } else {
        printf("1");
      }
  }
}

/*************************************************
  Function: integerQuickSort
  Description: Sorts the integer array using the quick sort algorithm

  Input   : int_array – Array with integer elements
            low – First element in the integer array
            high – Last element in the integer array
  Output  : void
*/
void integerQuickSort(int *int_array, int low, int high) {
  int pivot, i, j;
  if(low < high) {
    pivot = low; // select pivot
    i = low;
    j = high;
    while(i < j) {
      // increment i till number is greater than pivot
      while(int_array[i] <= int_array[pivot] && i <= high)
        i++;
      // decrement j till number is less than pivot
      while(int_array[j] > int_array[pivot] && j >= low)
        j--;
      // swap the elements using bit-wise XOR operator
      if(i < j) {
        int_array[i] = int_array[i] ^ int_array[j];
        int_array[j] = int_array[i] ^ int_array[j];
        int_array[i] = int_array[i] ^ int_array[j];
      }
    }
    // swap the elements using bit-wise XOR operator, if the element doesn't equal itself
    if (int_array[pivot] != int_array[high]) {
      int_array[pivot] = int_array[pivot] ^ int_array[j];
      int_array[j] = int_array[pivot] ^ int_array[j];
      int_array[pivot] = int_array[pivot] ^ int_array[j];
    }

    // recursively sort the subarrays by divide and conquer methods
    integerQuickSort(int_array, low, j - 1);
    integerQuickSort(int_array, j + 1, high);
  }
}
