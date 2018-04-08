#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>

void printDir(const char *path, int indent);

int main( int argc, char **argv){
    if(argc == 1){
        printf("UnixLS needs one path input");
        return 1;
    }
    printf("num argument: %d\n", argc);
    
    return 0;
}
