#include <string.h>
#include <pwd.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
 
 
#define PATH_TO_STATUS_SIZE 100
#define SIZE_STR_STATUS_FILE 1000
#define PROC_FILE_PARMATER_SZIE 100
#define UID_STRING_SIZE 20
 
 
void scanProcDir(){
    DIR *procDirPtr;
    struct dirent *direntPtr;
    procDirPtr = opendir("/proc");
    if (procDirPtr == NULL){
        perror("Error opendir /proc");
        exit(EXIT_FAILURE);
    }
 
    while( (direntPtr = readdir(procDirPtr)) != NULL){
        int fd;
        int uid;
        ssize_t readResutl;
        char programName[PROC_FILE_PARMATER_SZIE];
        char pathToStatusFile[PATH_TO_STATUS_SIZE];
        char strBuff[SIZE_STR_STATUS_FILE];
        char ppid[UID_STRING_SIZE];
        
        if(isdigit(direntPtr->d_name[0]) == 0)
            continue;
        
        strcpy(pathToStatusFile, "/proc/");
        strcat(pathToStatusFile,direntPtr->d_name);
        strcat(pathToStatusFile,"/status");
        char * tokenPtr;
        fd = open(pathToStatusFile, O_RDONLY);
        if(fd == -1){
            if(errno == ENOENT)
                continue;
            else{
                printf("Error open file %s - %s", pathToStatusFile, strerror(errno));
            }
        }
 
        while( (readResutl = read(fd, strBuff, SIZE_STR_STATUS_FILE)) != 0 ){
            tokenPtr = strtok(strBuff, "\n");
            while(tokenPtr != NULL){
                if (strstr(tokenPtr, "Name:") != NULL){
                    tokenPtr +=5; //Name: ==5
                    while(isspace(*tokenPtr) != 0)
                        tokenPtr++;
                    strcpy(programName, tokenPtr); 
                    //puts(programName); 
                } 
                if (strstr(tokenPtr, "PPid:") != NULL){
                    tokenPtr +=5; //PPid: ==5
                    while(isspace(*tokenPtr) != 0)
                        tokenPtr++;
                    strcpy(ppid, tokenPtr); 
                }             
                tokenPtr = strtok(NULL, "\n"); 
            }
        } 
        
        printf("%s %s %s\n", programName, direntPtr->d_name, ppid);        
         
        if (readResutl == -1){
            printf("Error read file %s %s\n", pathToStatusFile , strerror(errno));
        }
        if (close(fd) == -1){
            printf("Error close file %s - %s\n", pathToStatusFile, strerror(errno));
        }
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