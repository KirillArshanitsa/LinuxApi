#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h> 

int my_dup(int oldfd){
    int newfd;
    newfd = fcntl(oldfd, F_DUPFD, oldfd);
    if (newfd == -1){
        perror("Error get newfd ");
        return -1;
    }
    else
        return newfd;
}

int my_dup2(int oldfd, int newfd){

    if(fcntl(oldfd, F_GETFL) == -1){
        errno = EBADF;
        return -1;
    }

    if(oldfd == newfd){
        return newfd;
    }
    else{
        close(newfd);
        newfd = fcntl(oldfd, F_DUPFD, newfd);
        if (newfd == -1){
            perror("Error copy wfd ");
            return -1;
        }
        return newfd;
    }
}


int main(){
    printf("my_dup = %d\n", my_dup(2));
    printf("my_dup2 = %d\n", my_dup2(2, 10));

    exit(EXIT_SUCCESS);
}