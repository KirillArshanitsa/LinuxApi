#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char * argv[]){
    int fd;
    char a = '0';
    long numBytes;
    numBytes = strtol(argv[2], NULL, 0); 
    if (numBytes == 0){
        perror("Error parse argumnet ");
        exit(EXIT_FAILURE);       
    }

    if (argc == 3){
        fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
        if (fd == -1){
            printf("Error open file %s ", argv[1]);
            puts(strerror(errno));
            exit(EXIT_FAILURE);
        }
        while(numBytes--){
            if(write(fd, &a, sizeof(a)) == -1){
                perror("Error write file ");
                exit(EXIT_FAILURE);
            }
        }

        if (close(fd) == -1){
            perror("Error close file ");
            exit(EXIT_FAILURE);          
        }
    }
    else if(argc == 4){
        fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd == -1){
            printf("Error open file %s ", argv[1]);
            puts(strerror(errno));
            exit(EXIT_FAILURE);
        } 
        while(numBytes--){
            if (lseek(fd, 0, SEEK_END) == -1){
                perror("Error seek in SEEK_END ");
                exit(EXIT_FAILURE);               
            }
            if(write(fd, &a, sizeof(a)) == -1){
                perror("Error write file ");
                exit(EXIT_FAILURE);
            }
        }

        if (close(fd) == -1){
            perror("Error close file ");
            exit(EXIT_FAILURE);          
        }              
    }
    else{
        puts("you must - atomic_append filename num-bytes [x] or atomic_append filename num-bytes");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS); 

}

//./atomic_append f1 1000000 & ./atomic_append f1 1000000     ./atomic_append f2 1000000 x & ./atomic_append f2 1000000 x