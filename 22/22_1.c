#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
 #include <unistd.h>

void processSigmanl(int sig, siginfo_t * sigInfo, void *ucontext){
    printf("Recive signal %d %s\n%d pid %d uid %d\n", sig, strsignal(sig), sigInfo->si_code, sigInfo->si_pid, sigInfo->si_uid);
}

int main(int argc, char * argv[]){

    struct sigaction act;
    sigset_t newSet;
    sigset_t oldSet;

    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = processSigmanl;
    sigemptyset(&act.sa_mask);

    if(sigaction(SIGCONT, &act, NULL) == -1){
        printf("Error sigaction %d %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    sigemptyset(&newSet);
    sigaddset(&newSet, SIGCONT);
    if (sigprocmask(SIG_BLOCK, &newSet, &oldSet) == -1){
        printf("Error sigprocmask %d %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);       
    }
    puts("SIG_BLOCK blocked and add handler");

    printf("%s %d\n", argv[0], getpid());
    puts("Stop process");
    raise(SIGSTOP);
    puts("Start after stop");
    puts("Unlock SIGCONT");

    if (sigprocmask(SIG_UNBLOCK, &newSet, NULL) == -1){
        printf("Error revert sigprocmask %d %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);       
    }   
    puts("SIGCONT unlocked");

    exit(EXIT_SUCCESS);

}