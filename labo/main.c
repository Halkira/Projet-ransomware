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

        ///entry is a struct so we can take the variable d_name with '->'
        printf("%s/%s\n", path, entry->d_name);

        ///d_type is for checking if it's a dir or not (DT_DIR for dir)
        ///strcomp is for comparing the string because when there is the file "." or ".." the recursivity doesn't work and make a loop
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            ///we don't know the lenght of the path so we're setting that to 250
            char path_file[250] = {};
            ///we concatenate the path to the path_files variable
            strcat(path_file, path);
            strcat(path_file, "/");
            strcat(path_file, entry->d_name);
            ///call the function for recursivity
            list_dir(path_file);
        }
    }

    closedir(directory);
}


int main(void)
{
    list_dir("/home/ubuntu/coding");

    return(0);

}