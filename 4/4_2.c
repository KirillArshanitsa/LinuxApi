#include<sys/stat.h>
#include<fcntl.h>
#include <stdlib.h>
#include <stdio.h> 
#include<unistd.h>
#define BUFFER_SIZE 1000


void copyFile(char * const src, char * const dest){
    char buffer[BUFFER_SIZE];
    int srcFd;
    int destFd;
    int readData;

    srcFd = open(src, O_RDONLY);
    if(srcFd == -1){
        printf("Error open source file %s ", src);
        perror("");
        exit(EXIT_FAILURE);
    }

    destFd = open(dest, O_CREAT | O_EXCL | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if(destFd == -1){
        printf("Error open dest file %s ", dest);
        perror("");
        exit(EXIT_FAILURE);
    }

    while((readData = read(srcFd, buffer, BUFFER_SIZE)) > 0){
        if (readData == -1){
            printf("Error read src file %s ", src);
            perror("");
            exit(EXIT_FAILURE);           
        }

        if(write(destFd, buffer, readData) == -1){
            printf("Error write dest file %s ", dest);
            perror("");
            exit(EXIT_FAILURE);               
        }
    }

    if(close(srcFd) == -1){
        printf("Error close file %s", src);
        perror("");
        exit(EXIT_FAILURE);         
    }

    if(close(destFd) == -1){
        printf("Error close file %s", dest);
        perror("");
        exit(EXIT_FAILURE);         
    }

}

void createFileHole( char * const path){
    int fd;
    char beginStr[]= "Begin file with hole\n";
    char endStr[]= "\nEnd file.";
    
    fd = open(path, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if(fd == -1){
        printf("Error create file %s ", path);
        perror("");
        exit(EXIT_FAILURE);
    }

    if( write(fd, beginStr, sizeof(beginStr)) != sizeof(beginStr)){
        printf("Error write file %s begin string ", path);
        perror("");
        exit(EXIT_FAILURE);
    }
    if (lseek(fd, 10000000, SEEK_CUR) == -1){
        printf("Error lseek file %s ", path);
        perror("");
        exit(EXIT_FAILURE);        
    }
    if( write(fd, endStr, sizeof(endStr)) != sizeof(endStr)){
        printf("Error write file %s end string ", path);
        perror("");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1){
        printf("Error close file %s", path);
        perror("");
        exit(EXIT_FAILURE);        
    }

}

int main(){
    char fileHole[]= "file_hole.txt";
    char fileCopy[] = "copy_file.txt";

    createFileHole(fileHole);
    copyFile(fileHole, fileCopy);
    exit(EXIT_SUCCESS); 

}