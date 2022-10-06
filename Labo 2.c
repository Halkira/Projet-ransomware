#include <stdio.h>

void printCharArray(char arr[], int size)
{
    for(int i = 0; i < size; i++)
        printf("%c\n", arr[i]);
}

int main(void)
{
    char arr[] = {'A', 'B', 'C', 'D'};
    printCharArray(arr, 4);
}