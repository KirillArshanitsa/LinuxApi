#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h> 

int main(int argc, char *argv[]){
    struct stat statbuf;
    mode_t mode;
    mode_t fileMode;

    for(int i = 1; i < argc ;i++){
        if (stat(argv[i], &statbuf) == -1){
            printf("Error stat %s %s\n",argv[i],  strerror(errno));
            exit(EXIT_FAILURE);
        }

        if(S_ISREG(statbuf.st_mode)){
            if( (statbuf.st_mode & S_IXUSR) | (statbuf.st_mode & S_IXGRP) | (statbuf.st_mode & S_IXOTH) )
                fileMode = 0555;
            else
                fileMode = 0444;
            mode = (statbuf.st_mode | fileMode); //a+r | a+rx
            if (chmod(argv[i], mode) == -1){
                printf("Error chmod folder %s %s\n",argv[i],  strerror(errno));
                exit(EXIT_FAILURE);                
            }    
        }
        else if(S_ISDIR(statbuf.st_mode)){
            mode = (statbuf.st_mode | 0555); //a+rx
            if (chmod(argv[i], mode) == -1){
                printf("Error chmod folder %s %s\n",argv[i],  strerror(errno));
                exit(EXIT_FAILURE);                
            }
        }
    }

}