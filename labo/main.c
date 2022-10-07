#include <stdio.h>
#include <dirent.h>

/// \param arr Array of Char
/// \param size Size of the array
/// \param i Index of the current iteration


/*
/// Fonction pour print un array

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
*/


int main(void)
{
    /*char arr[] = {'A', 'B', 'C', 'D'};
    printCharArray(arr, 4, 0);*/

    /// Code recopié depuis c-for-dummies.com

    DIR *folder;
    struct dirent *entry;
    int files = 0;

    folder = opendir(".");
    if(folder == NULL)
    {
        perror("Unable to read directory");
        return(1);
    }

    while( (entry=readdir(folder)) )
    {
        files++;
        printf("File %3d: %s\n",
               files,
               entry->d_name
        );
    }

    closedir(folder);

    return(0);



}