#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN_BUF_SIZE 1024


int main(int argc, char *argv[]){
    int readCountStr;
    int fd;
    char *pathToFile;
    char *bufferPtr;
    off_t seekEndSize;
    off_t bufferSize;
    off_t seekCurrent;
    char *newStrPtr; 

    readCountStr = atoi(argv[1]);
    pathToFile = argv[2];
    
    fd = open(pathToFile, O_RDONLY); 
    if(fd == -1){
        printf("Error open file %s %s\n", pathToFile, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if( (seekEndSize = lseek(fd, 0, SEEK_END)) == -1){
        printf("Error SEEK_END file %s %s\n", pathToFile, strerror(errno));
        exit(EXIT_FAILURE);        
    }

    if (seekEndSize > MIN_BUF_SIZE)
        bufferSize = MIN_BUF_SIZE;
    else
        bufferSize = seekEndSize;

    bufferPtr = (char *) malloc(bufferSize);

    if(lseek(fd, -bufferSize, SEEK_CUR) == -1){
        printf("Error SEEK_CUR file %s %s\n", pathToFile, strerror(errno));
        exit(EXIT_FAILURE);            
    }



    for(int strCount = readCountStr; strCount > 0;){
        if(read(fd, bufferPtr, bufferSize) == -1){
            printf("Error read data from file %s %s\n", pathToFile, strerror(errno));
            exit(EXIT_FAILURE);            
        }
    
        printf("%s", bufferPtr);
        // newStrPtr = bufferPtr;
        // newStrPtr = strrchr(newStrPtr, '\n'); 
        // if(newStrPtr != NULL){
        //     printf("%s", newStrPtr);
        //     strCount--;   
        //     if(strCount < 0)
        //         break;  
        //     while((newStrPtr = strrchr(newStrPtr, '\n')) != NULL){
        //         printf("%s", newStrPtr);
        //         strCount--;   
        //         if(strCount < 0)
        //             break;  
        //     }
        // }
        // else{
        //     printf("%s", bufferPtr);
        // }

        if(lseek(fd, -bufferSize, SEEK_CUR) == -1){
            printf("Error SEEK_CUR file %s %s\n", pathToFile, strerror(errno));
            exit(EXIT_FAILURE);            
        }

        seekEndSize = seekEndSize - bufferSize;
        if (seekEndSize > MIN_BUF_SIZE)
            bufferSize = MIN_BUF_SIZE;
        else
            bufferSize = seekEndSize;

        if(lseek(fd, -seekEndSize, SEEK_CUR) == -1){
            printf("Error SEEK_CUR file %s %s\n", pathToFile, strerror(errno));
            break;
            //exit(EXIT_FAILURE);            
        }
    }

    free(bufferPtr);
    if(close(fd) == -1){
        printf("Error close file %s %s\n", pathToFile, strerror(errno));
        exit(EXIT_FAILURE);           
    }


    puts("");
    exit(EXIT_SUCCESS);  
}