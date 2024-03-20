
#include <libgen.h>        /* Для объявления basename() */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define _POSIX_C_SOURCE 199309
#include <signal.h>

#define CMD_SIZE 200

int main(int argc, char *argv[])
{
    char cmd[CMD_SIZE];
    pid_t childPid;
    sigset_t sigSet;
    int signal;

    sigemptyset(&sigSet);
    sigaddset(&sigSet, SIGUSR1);
    sigaddset(&sigSet, SIGUSR2);
    if(sigprocmask(SIG_BLOCK, &sigSet, NULL) == -1){
        perror("Error sigprogmask");
        exit(EXIT_FAILURE);       
    }

    setbuf(stdout, NULL);   /* Отключаем буферизацию стандартного вывода */
    printf("Parent PID=%ld\n", (long) getpid());
    switch (childPid = fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:         /* Потомок немедленно завершается, чтобы стать «зомби» */
        printf("Child (PID=%ld) exiting\n", (long) getpid());
        puts("Send signal to child");
        sleep(3);
        if(kill(getppid(), SIGUSR2) == -1){
            perror("Error seng signal to parrent");
            _exit(EXIT_FAILURE);
        }

        if(kill(getppid(), SIGUSR1) == -1){
            perror("Error seng signal to parrent");
            _exit(EXIT_FAILURE);
        }
        _exit(EXIT_SUCCESS);
    default:        /* Родитель */
        //sleep(3);   /* Даем потомку шанс начать выполнение и завершиться */
        sigdelset(&sigSet, SIGUSR2);
        signal = sigwaitinfo(&sigSet, NULL);
        printf("Recive signal from parrent %d\n", signal);
        snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
        system(cmd);    /* Видим потомка-«зомби» */
        /* Теперь отправляем «зомби» сигнал о безусловном завершении */
        if (kill(childPid, SIGKILL) == -1)
            perror("kill");
        sleep(3);    /* Даем потомку шанс отреагировать на сигнал */
        printf("After sending SIGKILL to zombie (PID=%ld):\n",
               (long) childPid);
        system(cmd);    /* Опять видим потомка-«зомби» */
        exit(EXIT_SUCCESS);
    }
}