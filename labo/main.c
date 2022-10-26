#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>


void list_dir(const char *path) {
    DIR *directory;
    struct dirent *entry;
    directory = opendir(path);
    printf("Reading files in: %s\n", path);

    while ((entry = readdir(directory)) != NULL) {

        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            unsigned short int lenght = strlen(path)+1+strlen(entry->d_name)+1;
            char *buffer = (char *)malloc(lenght * sizeof(char));
            if (buffer != NULL){
                snprintf(buffer, lenght, "%s/%s", path, entry->d_name);
                list_dir(buffer);
                free(buffer);
            }
        } else if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
            printf("%s/%s\n", path, entry->d_name);
        }
    }

    closedir(directory);
}

int main(int argc, char * argv[]) {

    printf("%d arguments \n", argc-1);

    for(int i = 1; i<argc;i++){
        printf("Path : %s\n", argv[i]);
        if (strlen(argv[i]) <= PATH_MAX){
            list_dir(argv[i]);
        }
    }
    return (0);

}