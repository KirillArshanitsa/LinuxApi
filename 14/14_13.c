#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>


#define TEMP_FILE_NAME_SIZE 8

void sort(char list[][PATH_MAX], int size){
    char tmpPtr[PATH_MAX];

    for(int e = size; e > 0; e--){
        for(int i = 0 ; i < e ; i++){
            if (strcmp(&list[i][0], &list[i + 1][0]) < 0){
                strcpy(tmpPtr, list[i]);
                strcpy(list[i], list[i + 1]);
                strcpy(list[i + 1], tmpPtr);
            }
        }
    }
}


int main(int argc, char* argv[]){
    clock_t start, finish;
    char tmpFileName[TEMP_FILE_NAME_SIZE];
    int fileCount;
    char path[PATH_MAX];
    char tmpPath[PATH_MAX];
    int fd;

    if(argc < 3){
        puts("Error - you must use program path to filder file count");
        exit(EXIT_FAILURE);
    } 
    fileCount =  atoi(argv[1]);
    char allFiles[fileCount][PATH_MAX];
    strcpy(path, argv[2]);
    strcat(path, "/");
    

    srand(time(0));
    for(int i = 0, t = 0; i < fileCount; i++, t = 0){
        sprintf(tmpFileName, "x%06d", 1 + rand() % fileCount);
        for(int u = 0; u < i; u++){
            if (strstr(allFiles[u], tmpFileName) != 0){
                i--;
                t = 1;
                break;
            }
         } 
        if(t)
            continue;
        strcpy(allFiles[i], tmpFileName);
        allFiles[i][PATH_MAX - 1] = '\0'; // 8 - 1 allFiles
        strcpy(tmpPath, path);
        strncat(tmpPath, allFiles[i], strlen(allFiles[i]));
        strcpy(allFiles[i], tmpPath);
    }

    sort(allFiles, fileCount);

    puts("Start create");
    start =  clock();
    for(int i = 0; i < fileCount; i++){
        fd = open(allFiles[i], O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);
        if(fd == -1){
            if(errno == EEXIST){
                printf("Error open file %s %s\n", allFiles[i], strerror(errno));
            }
            exit(EXIT_FAILURE);
        }
        if (write(fd, "s", 1) == -1){
            printf("Error write file %s %s\n", allFiles[i], strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (close(fd) == -1){
            printf("Error close file %s %s\n", allFiles[i], strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    finish =  clock();
    printf("Created %d files, time = %lg\n", fileCount, (double)(finish - start) / CLOCKS_PER_SEC);

    start =  clock();
    //sort(allFiles, fileCount);
    for(int i = 0; i < fileCount; i++){
        remove(allFiles[i]);
    }
    finish =  clock();
    printf("Deleted %d, files time = %lg\n", fileCount, (double)(finish - start) / CLOCKS_PER_SEC);

    exit(EXIT_SUCCESS);

}