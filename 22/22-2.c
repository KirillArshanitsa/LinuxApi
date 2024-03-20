#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

siginfoHandler(int sig, siginfo_t *si, void *ucontext){
    printf("Signal %d %s\tcode %d, uid %d, pid %d\n", sig , strsignal(sig), si->si_code, si->si_uid, si->si_pid);
}


int main(int argc, char * argv[]){
    struct sigaction act;
    sigset_t sigNewBlockSet;
    int sig, i;

    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = siginfoHandler;

    for(sig = 1; sig < NSIG ;sig++){
        if (sigaction(sig, &act, NULL) == -1){
            printf("Error sigaction signal %d %s %d %s\n", sig, strsignal(sig), errno, strerror(errno));
        }
    }

    sigfillset(&sigNewBlockSet);
    if(sigprocmask(SIG_BLOCK, &sigNewBlockSet, NULL) == -1){
        printf("Error sigprocmask SIG_BLOCK %d %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);       
    }

    for(i = SIGRTMIN; SIGRTMIN + i <=  SIGRTMAX  ;i++){
         if (sigaction(SIGRTMIN + i, &act, NULL) == -1){
            printf("Error sigaction signal %d %s %d %s\n", SIGRTMIN + i, strsignal(SIGRTMIN + i), errno, strerror(errno));
        }       
    }

    printf("%s %d\n", argv[0], getpid());
    puts("All signals locked\nStart sleep");
    sleep(15);
    puts("Stop sleep");
    if(sigprocmask(SIG_UNBLOCK, &sigNewBlockSet, NULL) == -1){
        printf("Error sigprocmask SIG_UNBLOCK %d %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);       
    }
    puts("Signal unlocked"); 
    puts("Finish");   
}


