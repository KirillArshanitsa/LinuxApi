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
 
uid_t getUserId(const char *username){
    struct passwd *pwdPtr;
    errno = 0;
    pwdPtr = getpwnam(username);
    if (pwdPtr == NULL){
        if(errno ==0){
            printf("Error user %s not found\n",username);
            return -1;
        }
        else{
            printf("Error get info about user %s - %s\n",username, strerror(errno));
            return -1;
        }
    }
    return pwdPtr->pw_uid;
}
 
void scanProcDir(uid_t userId){
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
        char uidUser[UID_STRING_SIZE];
        
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
            
                if (strstr(tokenPtr, "Uid:") != NULL){
                    tokenPtr +=4; //Uid: ==4
                    char *strPtr = tokenPtr;
                    int i = 0;
                    while(isspace(*tokenPtr) != 0)
                        tokenPtr++;
 
                    strPtr = tokenPtr;  
                    while(isspace(*strPtr) == 0){
                        strPtr++;
                        i++;
                    }      
                    strncpy(uidUser, tokenPtr, i); 
                    uidUser[i] = '\0';
                    //puts(uidUser);      
                }
                tokenPtr = strtok(NULL, "\n"); 
            }
        }
        //printf("%s %s %d\n", uidUser, programName, atoi(uidUser), uid);
 
        if(userId == atoi(uidUser)){
            printf("%s %s\n", programName, direntPtr->d_name);        
        }  
 
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
    char userName [] = "ansible";
    uid_t uid;
    uid = getUserId(userName);
    if(uid == -1)
        exit(EXIT_FAILURE);
 
    scanProcDir(uid);
 
    exit(EXIT_SUCCESS);
}