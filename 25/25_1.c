#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    pid_t childPid;
    int status = 0;

    setbuf(stdout, NULL);
    childPid = fork();
    switch (childPid)
    {
    case -1:
        perror("Error fork()");
        exit(EXIT_FAILURE);
    case 0:
        sleep(4);
        puts("Child process run exit(-1)");
        exit(-1);
    default:
        if(wait(&status) == -1){
            perror("Error wait()");
            exit(EXIT_FAILURE);            
        }
        printf("Child send code %d\n", status);
        exit(EXIT_SUCCESS);
    }

}