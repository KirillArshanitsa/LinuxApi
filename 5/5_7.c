#include <stdlib.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

ssize_t my_readv(int fd, const struct iovec * iov, int iovcnt){
    ssize_t readenSize = 0;
    for(int currentRead = 0, i = 0; i < iovcnt; i ++, currentRead = 0){
        currentRead =  read(fd, iov[i].iov_base, iov[i].iov_len);
        if (currentRead == -1){
            perror("Error readr iovec data");
            exit(EXIT_FAILURE);
        }
        readenSize +=  currentRead;     
    }
    return readenSize;
}


ssize_t my_writev(int fd, const struct iovec * iov, int iovcnt){
    ssize_t writenSize = 0;
    for(int currentWrite = 0, i = 0; i < iovcnt; i++, currentWrite = 0){
        currentWrite = write(fd, iov[i].iov_base, iov[i].iov_len);
        if (currentWrite == -1){
            perror("Error write iovec data");
            exit(EXIT_FAILURE);
        }
        writenSize += currentWrite;
    }
    return writenSize;
}



int main(int argc, char * argv[]){
    int totalSize = 0;
    int readenSize, writenSize;

    int fd;
    char str1[] = "My test string for my_readv and my_writev!";
    double num1 = 199.578;
    char str2[] = "Finish string!";

    struct iovec myarray[3];
    myarray[0].iov_base = str1;
    myarray[0].iov_len = sizeof(str1);
    totalSize += myarray[0].iov_len;
    myarray[1].iov_base = &num1;
    myarray[1].iov_len = sizeof(num1);
    totalSize += myarray[1].iov_len;
    myarray[2].iov_base = str2;
    myarray[2].iov_len = sizeof(str2);
    totalSize += myarray[2].iov_len;

    fd = open("my_iovec_file.txt", O_CREAT | O_TRUNC | O_WRONLY, S_IWUSR | S_IRUSR);
    if(fd == -1){
        perror("Error open write file my_iovec_file.txt");
        exit(EXIT_FAILURE);
    }
    printf("Inserting data = %d\n", totalSize);
    readenSize = my_writev(fd, myarray, 3);

    if(readenSize == totalSize){
        puts("Write all data");
    }
    else{
        printf("Error - writen %d mustbe writen - %d\n", readenSize, totalSize);
    }

    if (close(fd) == -1){
        perror("Error close file");
        exit(EXIT_FAILURE);
    }


    puts("Clean data");
    num1 = 0;
    str1[0] = '\0';
    str2[0] = '\0';
    
    printf("num1 = %f\n", num1);
    printf("str1 = %s\n", str1);
    printf("str2 = %s\n", str2);

    puts("Read data:");

    fd = open("my_iovec_file.txt", O_RDONLY);
    if(fd == -1){
        perror("Error open read file my_iovec_file.txt");
        exit(EXIT_FAILURE);       
    }
    writenSize = my_readv(fd, myarray, 3);
    if(writenSize == -1){
        perror("Error read file my_iovec_file.txt");
        exit(EXIT_FAILURE);        
    }
    puts("Print data:");
    printf("Read data from file - %d\n", writenSize);

    printf("num1 = %f\n", num1);
    printf("str1 = %s\n", str1);
    printf("str2 = %s\n", str2);


    //printf("%s\n",(*((char *)myarray[0].iov_base)));
    //printf("%f\n", *(double*)myarray[1].iov_base);
    //printf("%s\n", (*((char *)myarray[2].iov_base)));
    if (close(fd) == -1){
        perror("Error close file");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}