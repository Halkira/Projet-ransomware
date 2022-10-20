#include <stdio.h>
#include <dirent.h>
#include <string.h> ///library for string comparison and manipulation


void list_dir(const char* path) {
    DIR *directory;
    struct dirent *entry;
    directory = opendir(path);
    printf("Reading files in: %s\n", path);

    ///Check if there is an entry or not
    while ((entry = readdir(directory)) != NULL) {

        ///d_type is for checking if it's a dir or not (DT_DIR for dir)
        ///strcomp is for comparing the string because when there is the file "." or ".." the recursive doesn't work and make a loop
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            unsigned short int lenght = (strlen(path)+1+strlen(entry->d_name)+1);
            char *buffer = (char *)malloc(lenght * sizeof(char));
            ///we don't know the lenght of the path so we're setting that to 250
            ///we concatenate the path to the path_files variable
            strcat(buffer, path);
            strcat(buffer, "/");
            strcat(buffer, entry->d_name);
            ///call the function for recursivity
            list_dir(buffer);
        } else {
            printf("%s/%s\n", path, entry->d_name);
        }
        free(list_dir);
    }

    closedir(directory);
}


int main(void)
{
    list_dir("/home/ubuntu/coding");

    return(0);

}