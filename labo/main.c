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

int isDir(struct dirent* ent){
    if ((strchr(ent->d_name, '.')) == NULL){
        return 1;
    } else {
        return 0;
    }
}

void list_dir(){
    DIR* directory = NULL;
    struct dirent* fichier = NULL;
    directory = opendir("/home/ubuntu/coding");

    while((fichier = readdir(directory)) != NULL){
        if (isDir(directory) == 1){
            printf("Fichier ");
        } else if (isDir(directory) == 0){
            printf("Dossier ");
        }
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