#include <stdio.h>
#include <dirent.h>
#include <string.h>

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


void list_dir(const char* path) {
    DIR *directory = NULL;
    struct dirent *entry = NULL;
    directory = opendir(path);

    while((fichier = readdir(directory)) != NULL){
        if (isDir(directory) == 1){
            printf("Fichier ");
        } else if (isDir(directory) == 0){
            printf("Dossier ");
        }
        printf("fichier lu '%s'\n", fichier->d_name);
    }

    closedir(directory);
}


int main(void)
{
    /*char arr[] = {'A', 'B', 'C', 'D'};
    printCharArray(arr, 4, 0);*/

    list_dir("/home/ubuntu/coding");

    return(0);

}