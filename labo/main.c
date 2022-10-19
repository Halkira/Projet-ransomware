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

void list_dir(){
    DIR* directory = NULL;
    struct dirent* fichier = NULL;
    directory = opendir("/home/ubuntu/coding");

    if(directory == NULL) {
        printf("Problème ouverture du dossier");
        exit(1);
    }

    while((fichier = readdir(directory)) != NULL){
        printf("fichier lu '%s'\n", fichier->d_name);
    }

    return 0;
}


int main(void)
{
    /*char arr[] = {'A', 'B', 'C', 'D'};
    printCharArray(arr, 4, 0);*/

    list_dir();

    return(0);



}