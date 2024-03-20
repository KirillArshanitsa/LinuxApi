#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


int main(){
    pid_t pid;

    setbuf(stdout, NULL);
    pid = fork();

    switch (pid)
    {
    case -1:
        perror("Error fork");
        exit(EXIT_FAILURE);
    case 0:
        printf("Child process pid %d\n", getpid());
        printf("Send parrent %d signal and sllep 6 sec\n", getppid());
        if (kill(getppid(), SIGQUIT) == -1){
            perror("Error send signal parrent");
            exit(EXIT_FAILURE);            
        }
        sleep(6);
        printf("Child process parrent is %d\n", getppid());

    default:
        puts("Parrent process sleep 3 sec");
        sleep(3);
        exit(EXIT_SUCCESS);
    }

}