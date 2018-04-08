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

void printFile_Normal(struct dirent *entry, int i);
void printFile_Long(struct dirent *entry, int i);
void printDir(const char *path, void (*f)(struct dirent*, int), int i, int R);

void transferModeToString(struct stat);

int main( int argc, char **argv){
    int l = 1;
    if(argc == 1){
        if(l == 0)
            printDir(".", printFile_Normal, 0, 0);
        else if(l == 1)
            printDir(".", printFile_Long, 0, 0);
        printf("\n");
        return 0;
    }
    
    printf("num argument: %d\n", argc);
    
    return 0;
}

void printFile_Normal(struct dirent *entry, int i){
    if(i==1)
        printf("%1lld  ", entry->d_ino);
    printf("%s\t", entry->d_name);
}

void printFile_Long(struct dirent *entry, int i){
    char buf[1024];
    struct stat fileStat;
    struct passwd *pw = NULL;
    struct group *grp;
    struct tm *time;
    
    lstat(entry->d_name, &fileStat);
    grp = getgrgid(fileStat.st_gid); 
    pw = getpwuid(fileStat.st_uid);
    time = gmtime(&fileStat.st_mtime);
    if(i==1)
        printf("%11llu", fileStat.st_ino);
    transferModeToString(fileStat);
    printf("%4d", fileStat.st_nlink);
    printf("%15s", pw->pw_name);
    printf("%8s", grp->gr_name);
    printf("%8lld\t", fileStat.st_size);
    printf("%s %02d %04d %02d:%02d\t", months[time->tm_mon], time->tm_mday, time->tm_year + 1900, time->tm_hour, time->tm_min);
    printf("%s", entry->d_name);
    if( entry->d_type == DT_LNK){
        readlink(entry->d_name, buf, sizeof(buf));
        printf(" -> %s", buf);
    }
    printf("\n");
}

void printDir(const char *path, void (*printFile)(struct dirent*, int), int i, int R) {
    DIR *dir;
    struct dirent *entry;

    if(!(dir = opendir(path)))
        return;

    // print current dir's files
    while ((entry = readdir(dir)) != NULL){
        if(entry->d_type == DT_REG || entry->d_type == DT_DIR || entry->d_type == DT_LNK){
            if( strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            printFile(entry, i);
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
                if( strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    continue;
                snprintf(nextPath, sizeof(nextPath), "%s/%s", path, entry->d_name);
                printf("\n\n%s:\n", nextPath);
                printDir(nextPath, printFile, i, R);
            }
        }   
        closedir(dir);
    }
}

void transferModeToString( struct stat fileStat){
    printf("  ");
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
