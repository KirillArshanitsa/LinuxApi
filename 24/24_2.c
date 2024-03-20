#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 100


void processStdin(char * const str){
    ssize_t readedSize;
    readedSize = read(STDIN_FILENO, str, BUFFER_SIZE);
    if(readedSize == -1){
        printf("Error read(STDIN_FILENO) %d %s\n", errno, strerror(errno));
        _exit(EXIT_FAILURE);             
    }
    str[readedSize + 1] = '\0';
    printf("Readed %zd  - %s", readedSize, str);
    puts("Close STDIN_FILENO");
    if(close(STDIN_FILENO) ==-1){
        printf("Error close(STDIN_FILENO) %d %s\n", errno, strerror(errno));
        _exit(EXIT_FAILURE);           
    }
    puts("Closed STDIN_FILENO");  
}


int main(){
    pid_t pid;
    char str[BUFFER_SIZE + 1];

    puts("Create process vfork");
    setbuf(stdout, NULL);
    pid = vfork();
    switch (pid)
    {
    case -1:
        printf("Error vfork %d %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    case 0:
        printf("Child process %d - try read stdin\n", getpid());
        processStdin(str);
        break;
    default:
        sleep(8);
        printf("Parent pid %d\nTry to read from STDIN_FILENO\n", getpid());
        processStdin(str);
    }

    puts("Finish\n");
    exit(EXIT_SUCCESS);

}