#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>

char * my_getcwd(char *cwdbuf, size_t size){
    DIR *dirPtr;
    struct dirent *direntPtr; 
    struct stat parrentStatBuf;
    struct stat currentStatBuf;
    size_t currentPathSize = 0;
    char *tempCharPtr;
    size_t patOfPathSize = 0;

    if(stat(".", &currentStatBuf) == -1){
        printf("Error stat . - %s\n", strerror(errno));
        exit(EXIT_FAILURE);       
    }

    if(stat("..", &parrentStatBuf) == -1){
        printf("Error stat .. - %s\n", strerror(errno));
        exit(EXIT_FAILURE);  
    }

    // cd .. == cd /..
    if( (currentStatBuf.st_dev == parrentStatBuf.st_dev) & (currentStatBuf.st_ino ==parrentStatBuf.st_ino) ){
        if (size < 2){
            errno = ERANGE;
            return NULL;
        }else{
            cwdbuf[0] = '/';
            cwdbuf[1] = '\0';       
            return cwdbuf;
        }
    }

    while( !((currentStatBuf.st_dev == parrentStatBuf.st_dev) & (currentStatBuf.st_ino == parrentStatBuf.st_ino)) ){
        if(chdir("..") == -1){
            printf("Error chdir .. - %s\n", strerror(errno));  
            exit(EXIT_FAILURE);        
        }

        dirPtr = opendir(".");
        if(dirPtr == NULL){
            printf("Error opendir .. - %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        errno = 0;
        while((direntPtr = readdir(dirPtr)) != NULL){
            if( (strcmp(direntPtr->d_name, ".") == 0) | (strcmp(direntPtr->d_name, "..") == 0) ){
                continue;
            }       
    
            if(lstat(direntPtr->d_name, &parrentStatBuf) == -1){
                printf("Error lstat %s -%s\n", direntPtr->d_name, strerror(errno));
                exit(EXIT_FAILURE);            
            }
            if (parrentStatBuf.st_mode & S_IFDIR){
                if((currentStatBuf.st_dev ==  parrentStatBuf.st_dev) & (currentStatBuf.st_ino == parrentStatBuf.st_ino)){                    
                    strcat(cwdbuf, direntPtr->d_name);
                    strcat(cwdbuf, "/");
                    currentPathSize = currentPathSize + 1+ strlen(direntPtr->d_name);
                    if (closedir(dirPtr) == -1){
                        printf("Error closedir .. - %s\n", strerror(errno));     
                        exit(EXIT_FAILURE);  
                    }
                    break;
                }
            }
            else
                continue;

        }
        if(errno != 0){
            printf("Error readdir %s - %s\n", strerror(errno), direntPtr->d_name); 
         exit(EXIT_FAILURE);    
        }


        if(stat(".", &currentStatBuf) == -1){
            printf("Error stat . - %s\n", strerror(errno));
            exit(EXIT_FAILURE);       
        }

        if(stat("..", &parrentStatBuf) == -1){
            printf("Error stat .. - %s\n", strerror(errno));
            exit(EXIT_FAILURE);  
        }
    }

    tempCharPtr = malloc(currentPathSize);
    if(tempCharPtr == NULL){
        printf("Error malloc\n");
        exit(EXIT_FAILURE);         
    }
    tempCharPtr[0] = '\0';

    for(int i = currentPathSize - 2 ; i >= 0; i--){
        if(cwdbuf[i] == '/'){
            strncat(tempCharPtr, &(cwdbuf[i]) ,patOfPathSize + 1);
            patOfPathSize = 0;          
        }
        else
            patOfPathSize++;
    }

    if(patOfPathSize > 0){
        strcat(tempCharPtr, "/");
        strncat(tempCharPtr, cwdbuf, patOfPathSize); 
    }

    memcpy(cwdbuf, tempCharPtr, currentPathSize);
    free(tempCharPtr);

    if(chdir(cwdbuf) == -1){
      printf("Error revert to work catalog fcdir %s - %s\n", strerror(errno), direntPtr->d_name); 
      exit(EXIT_FAILURE); 
    }

    if (currentPathSize > size){
        errno = ERANGE;
        return NULL;
    }
    else
        return cwdbuf;
}


int main(){
    char cwd[PATH_MAX]; 
    if (my_getcwd(cwd, PATH_MAX) == NULL){
        printf("Error my_getcwd - %s\n", strerror(errno));
        exit(EXIT_FAILURE); 
    }
    else{
        printf("%s\n", cwd);
        exit(EXIT_SUCCESS);        
    }

}