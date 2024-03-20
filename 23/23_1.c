#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

volatile char flag;

void handler(int sig, siginfo_t *siginfo, void *ucontext){
    flag = 1;
    printf("Reive signal %d %s\n", sig, strsignal(sig));
}

unsigned int my_alarm(unsigned int seconds){
    struct itimerval new_value;
    struct itimerval old_value;
    new_value.it_value.tv_sec = seconds;
    new_value.it_value.tv_usec = 0;
    new_value.it_interval.tv_sec = 0;
    new_value.it_interval.tv_usec = 0;

    if(setitimer(ITIMER_REAL, &new_value, &old_value) == -1){
        perror("Error setitimer");
        exit(EXIT_FAILURE);
    }

    return old_value.it_value.tv_sec;

}


int main(){
    struct sigaction action;
    action.sa_sigaction = handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    if(sigaction(SIGALRM, &action, NULL) == -1){
        perror("Error sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Run alarm() = %u\n", alarm(5));
    printf("Run my_alarm() = %u\n", my_alarm(8));
    while(flag == 0){
        ;
    }
    puts("exit");
    exit(EXIT_SUCCESS);
}