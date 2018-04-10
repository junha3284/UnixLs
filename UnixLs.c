#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void printDir(const char *path, void (*f)(struct dirent*, const char*, int), int i, int R);
void printFile_Normal(struct dirent *entry, const char*, int i);
void printFile_Long(struct dirent *entry, const char*, int i);

// print stat file' permission string
void transferModeToString(struct stat);

int main( int argc, char **argv){
    int l = 0;
    int i = 0;
    int r = 0;

    // If user did not give any arugment, just UnixLs current path without any option 
    if(argc == 1){
        printDir(".", printFile_Normal, i, r);
        printf("\n");
        return 0;
    }
  
    // check the last argument from user 
    // if k == 1, the last arugment is the path which the user want to excute UnixLs on
    // otherwise, (k==0), the last argument is the for the option
    //                and the current will be the path argument for UnixLs
    int k=1;

    if(argv[argc-1][0] == '-')
       k=0; 

    for(int a=1; a < argc-k; a++){
        if(argv[a][0]!='-')
            printf("The arguments are not in right format\n");

        for(int j=1; argv[a][j]!='\0'; j++){
            if(argv[a][j] == 'l')
                l = 1;
            else if(argv[a][j] == 'i')
                i = 1;
            else if(argv[a][j] == 'R')
                r = 1;
            else {
                printf("there is no such option: %c\n", argv[a][j]); 
                return 1;
            }
        }
    }

    if(k ==1 ){
        if( l == 0){
            printDir(argv[argc-1], printFile_Normal, i, r);
            printf("\n");
            return 0;
        }
        printDir(argv[argc-1], printFile_Long, i, r);
        return 0;
    }

    // k == 0, which means the current path is the UnixLs target
    if(l == 0){
        printDir(".", printFile_Normal, i, r);
        printf("\n");
        return 0;
    }
    printDir(".", printFile_Long, i, r); 
        
  
    return 0;
}

void printDir(const char *path, void (*printFile)(struct dirent*, const char*, int), int i, int R) {
    DIR *dir;
    struct dirent *entry;

    if(!(dir = opendir(path))){
        printf("there is no such dir or file: %s\n", path);
        return;
    }

    // print current dir's files
    while ((entry = readdir(dir)) != NULL){
        if(entry->d_type == DT_REG || entry->d_type == DT_DIR || entry->d_type == DT_LNK){
            if( entry->d_name[0] == '.')
                continue;
            printFile(entry, path, i);
        }
    }
    closedir(dir);

    // print subdir's files
    if (R == 1){
        if(!(dir = opendir(path)))
            return;
        while ((entry = readdir(dir)) != NULL){
            if (entry->d_type == DT_DIR) {
                char nextPath[1024];
                if( entry->d_name[0] == '.')
                    continue;
                snprintf(nextPath, sizeof(nextPath), "%s/%s", path, entry->d_name);
                printf("\n\n%s:\n", nextPath);
                printDir(nextPath, printFile, i, R);
            }
        }   
        closedir(dir);
    }
}

void printFile_Normal(struct dirent *entry, const char *path, int i){
    if(i==1)
        printf("%lu ", entry->d_ino);
    printf("%s  ", entry->d_name);
}

void printFile_Long(struct dirent *entry, const char *path, int i){
    char filePath[1024];
    char buf[1024];

    struct stat fileStat;
    struct passwd *pw = NULL;
    struct group *grp = NULL;
    struct tm *time = NULL;
    
    snprintf(filePath, sizeof(filePath), "%s/%s", path, entry->d_name);
    lstat(filePath, &fileStat);
    grp = getgrgid(fileStat.st_gid); 
    pw = getpwuid(fileStat.st_uid);
    time = localtime(&fileStat.st_mtime);
    
    if(i==1)
        printf("%10lu", fileStat.st_ino);
    
    transferModeToString(fileStat);
    printf("%4lu", fileStat.st_nlink);
    printf("%10s ", pw->pw_name);
    printf("%8s", grp->gr_name);
    printf("%8lu", fileStat.st_size);
    printf("%7s %02d %04d %02d:%02d\t", months[time->tm_mon], time->tm_mday, time->tm_year + 1900, time->tm_hour, time->tm_min);
    printf("%s", entry->d_name);
    
    if( entry->d_type == DT_LNK){
        readlink(entry->d_name, buf, sizeof(buf));
        printf(" -> %s", buf);
    }

    printf("\n");
}

void transferModeToString( struct stat fileStat){
    printf(" ");
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
} 
