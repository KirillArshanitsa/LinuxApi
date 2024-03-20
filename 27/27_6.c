#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>


void handler(int sig)
{
    printf("Received signal %d (%s), signal mask is:\n", sig, strsignal(sig));
}

int main()
{
    sigset_t blockMask, origMask;
    struct sigaction sa;
    pid_t childPid;
    int status = 0;

    sigemptyset(&blockMask);      /* Блокируем SIGCHLD */
    sigaddset(&blockMask, SIGCHLD);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1){
        puts("sigaction");
        exit(EXIT_FAILURE);
    }
    sigprocmask(SIG_BLOCK, &blockMask, &origMask);

    switch (childPid = fork()) {
    case -1:          /* Вызов fork() завершился неудачей */
        perror("Fork");
        exit(EXIT_FAILURE);        
    case 0:           /* Ветка для потомка после успешного вызова fork() */
        puts("Chaild process");
        _exit(1);
    default:          /* Ветка для родителя, после успешного вызова fork() */
       sleep(8);
       if (wait(&status) == -1){
            perror("waitid");
            exit(EXIT_FAILURE);
       }
       puts("unlock SIGCHLD");
       sigprocmask(SIG_UNBLOCK, &origMask, NULL);
       puts("unlocked SIGCHLD");
        printf("Status child process %d\n", status);
       puts("Finish main process");
    }


    


}