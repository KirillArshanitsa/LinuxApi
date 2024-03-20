#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
 
#ifndef BUF_SIZE /* Позволяет "cc -D" перекрыть определение */
#define BUF_SIZE 1024
#endif
 
int main(int argc, char *argv[])
{
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];
    if (argc != 3 || strcmp(argv[1], "--help") == 0){
        printf("%s old-file new-file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    /* Открытие файлов ввода и вывода */
    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1){
        printf("opening file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; /* rw-rw-rw- */
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1){
        printf("opening file %s\ndf -h", argv[2]);
        exit(EXIT_FAILURE);
    }
     
    /* Перемещение данных до достижения конца файла ввода или возникновения ошибки */
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
        if (write(outputFd, buf, numRead) != numRead){
            puts("couldn't write whole buffer");
            exit(EXIT_FAILURE);
        }
        
    if (numRead == -1){
        puts("read");
        exit(EXIT_FAILURE);
    }
    if (close(inputFd) == -1){
        puts("close input");
       exit(EXIT_FAILURE);
    }
    if (close(outputFd) == -1){
        puts("close output");
       exit(EXIT_FAILURE);
    }        
    exit(EXIT_SUCCESS);
}