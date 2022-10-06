#include <stdio.h>

/// \param arr Array of Char
/// \param size Size of the array
/// \param i Index of the current iteration

void printCharArray(char arr[], int size, unsigned int i)
{
    if (i < size){
        printf("%c\n", arr[i]);
        i++;
    }
    if (i != size){
        printCharArray(arr, size, i);
    }
}

int main(void)
{
    char arr[] = {'A', 'B', 'C', 'D'};
    printCharArray(arr, 4, 0);
}