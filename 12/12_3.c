#include <string.h>
#include <pwd.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <limits.h>
 
 
#define PATH_TO_STATUS_SIZE 100
#define SIZE_STR_STATUS_FILE 1000
#define PROC_FILE_PARMATER_SZIE 100
#define UID_STRING_SIZE 20
 



void scanFdDir(const char * path){
    DIR *fdDirPtr;
    struct dirent *direntPtr;
    char pathToFd[PATH_MAX];
    ssize_t linkSize;
    char linkData[PATH_MAX];

    fdDirPtr = opendir(path);
    if (fdDirPtr == NULL){
        printf("Error opendir %s %d\n",path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("\t");
    while( (direntPtr = readdir(fdDirPtr)) != NULL){
        if( (strcmp(direntPtr->d_name, ".") == 0) || (strcmp(direntPtr->d_name, "..") == 0) ){
            continue;
        }

        linkData[0] = '\0';

        stpcpy(pathToFd, path);
        strcat(pathToFd,"/");
        strcat(pathToFd, direntPtr->d_name);
        //puts(pathToFd);

        linkSize = readlink(pathToFd, linkData, PATH_MAX);
        if (linkSize == -1){
            printf("Error readlink %s\n",pathToFd );
            continue;
        }

        linkData[linkSize] = '\0';
        printf("%s ", linkData);

    }

    if (closedir(fdDirPtr) == -1){
        printf("Error close %s %d\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    puts("");

} 
 
void scanProcDir(){
    DIR *procDirPtr;
    struct dirent *direntPtr;
    procDirPtr = opendir("/proc");
    if (procDirPtr == NULL){
        perror("Error opendir /proc");
        exit(EXIT_FAILURE);
    }
 
    while( (direntPtr = readdir(procDirPtr)) != NULL){
        char programName[PROC_FILE_PARMATER_SZIE];
        char pathToStatusFile[PATH_TO_STATUS_SIZE];
        char strBuff[SIZE_STR_STATUS_FILE];
        char ppid[UID_STRING_SIZE];
        
        if(isdigit(direntPtr->d_name[0]) == 0)
            continue;
        
        strcpy(pathToStatusFile, "/proc/");
        strcat(pathToStatusFile,direntPtr->d_name);
        strcat(pathToStatusFile,"/fd");

        printf("%s:\n", direntPtr->d_name);    
        scanFdDir(pathToStatusFile);    
         
    }
    
    if (closedir(procDirPtr) == -1){
        perror("Error closedir ");
        exit(EXIT_FAILURE);
    }
 
}


 
 
int main(int argc, char** argv){
 
    scanProcDir();
    exit(EXIT_SUCCESS);
}