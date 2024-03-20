#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



int main(){
    pid_t pid;

    pid = fork();
    switch (pid)
    {
    case -1:
        perror("Error fork");
        exit(EXIT_FAILURE);        
    case 0:
        abort();

    default:
        puts("Run process");
        break;
    }

    exit(EXIT_SUCCESS);
}