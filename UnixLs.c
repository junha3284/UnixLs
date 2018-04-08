#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>

void printFile_Normal(struct dirent *entry);
void printDir_R(const char *path, void (*f)(struct dirent*));

int main( int argc, char **argv){
    if(argc == 1){
        printDir_R(".", printFile_Normal);
        printf("\n");
        return 0;
    }
    printf("num argument: %d\n", argc);
    
    return 0;
}

void printFile_Normal(struct dirent *entry){
    printf("%s\t", entry->d_name);
}

void printDir_R(const char *path, void (*printFile)(struct dirent*)) {
    DIR *dir;
    struct dirent *entry;

    if(!(dir = opendir(path)))
        return;

    // print current dir's files
    while ((entry = readdir(dir)) != NULL){
        if(entry->d_type == DT_REG || entry->d_type == DT_DIR || entry->d_type == DT_LNK)
            printFile(entry);
    }
    closedir(dir);

    // print subdir's files
    if(!(dir = opendir(path)))
        return;
    while ((entry = readdir(dir)) != NULL){
        if (entry->d_type == DT_DIR || entry->d_type == DT_LNK) {
            char nextPath[1024];
            if( strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                 continue;
            snprintf(nextPath, sizeof(nextPath), "%s/%s", path, entry->d_name);
            printf("\n\n%s:\n", nextPath);
            printDir_R(nextPath, printFile);
        }
    }   
    closedir(dir);
}
