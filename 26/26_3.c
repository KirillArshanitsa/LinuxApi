#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

    /* Анализируем статус wait() с помощью макросов W* */
void printWaitStatus(const char *msg, int status)
{
    if (msg != NULL)
        printf("%s", msg);
    if (WIFEXITED(status)) {
        printf("child exited, status=%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("child killed by signal %d (%s)",
            WTERMSIG(status), strsignal(WTERMSIG(status)));
#ifdef WCOREDUMP    /* Не входит в стандарт SUSv3, может отсутствовать
                       в некоторых системах */
    if (WCOREDUMP(status))
        printf(" (core dumped)");
#endif
        printf("\n");
    } else if (WIFSTOPPED(status)) {
        printf("child stopped by signal %d (%s)\n",
            WSTOPSIG(status), strsignal(WSTOPSIG(status)));
#ifdef WIFCONTINUED  /* Входит в стандарт SUSv3, но может отсутствовать
                        в ранних версиях Linux и некоторых реализациях UNIX */
    } else if (WIFCONTINUED(status)) {
        printf("child continued\n");
#endif
    } else {    /* Этого никогда не должно случиться */
        printf("what happened to this child? (status=%x)\n",
        (unsigned int) status);
} 
}


void printWaitIdStatus(siginfo_t *infoptr){
    printf("si_code = %s\n", infoptr->si_code == CLD_EXITED ? "CLD_EXITED": infoptr->si_code == CLD_KILLED ? "CLD_KILLED": infoptr->si_code == CLD_STOPPED ? "CLD_STOPPED":
    infoptr->si_code == CLD_CONTINUED ? "CLD_CONTINUED": "Error status");
    printf("si_pid %d si_status %d si_uid %d\n", infoptr->si_pid, infoptr->si_status, infoptr->si_uid);
}

int
main(int argc, char *argv[])
{
    //int status;
    //pid_t childPid;
    siginfo_t infop;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        printf("%s [exit-status]\n", argv[0]);

    switch (fork()) {
    case -1: 
        perror("fork");
        exit(EXIT_FAILURE); 

    case 0:             /* Child: either exits immediately with given
                           status or loops waiting for signals */
        printf("Child started with PID = %ld\n", (long) getpid());
        if (argc > 1)                   /* Status supplied on command line? */
            exit(atoi(argv[1]));
        else                            /* Otherwise, wait for signals */
            for (;;)
                pause();
        exit(EXIT_FAILURE);             /* Not reached, but good practice */

    default:            /* Parent: repeatedly wait on child until it
                           either exits or is terminated by a signal */

        for (;;) {
            //childPid = waitpid(-1, &status, WUNTRACED
            if(waitid(P_ALL, 0, &infop ,WEXITED
#ifdef WCONTINUED       /* Not present on older versions of Linux */
                                                | WCONTINUED
#endif
            ) == -1){
                puts("waitid");
                exit(EXIT_FAILURE);
            }

            /* Print status in hex, and as separate decimal bytes */

            printf("waitpid() returned: PID=%ld\n",
                    (long) infop.si_pid);
            printWaitIdStatus(&infop);

            if(infop.si_status == CLD_EXITED){
                 exit(EXIT_SUCCESS);
            }
            // if (WIFEXITED(status) || WIFSIGNALED(status))
            //     exit(EXIT_SUCCESS);
        }
    }
}