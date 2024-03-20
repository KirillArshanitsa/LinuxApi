//#define _POSIX_C_SOURCE 199309L

#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

volatile char pass;

void handler(int sig, siginfo_t *siginfo, void *ucontext){
    printf("Recived signal %d %s value = %d\n", sig, strsignal(sig), siginfo->si_value.sival_int);
    pass = 1;
}

int main(){
    struct itimerspec itv;
    struct sigaction act;
    struct sigevent evp;
    timer_t timerid;

    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handler;

    itv.it_value.tv_sec = 5;
    itv.it_value.tv_nsec = 0;
    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_nsec = 0;

    if (sigaction(SIGALRM, &act, NULL) == -1){
        perror("sigaction");
        exit(EXIT_FAILURE);        
    }

    if(timer_create(SIGEV_SIGNAL, NULL, &timerid) == -1){
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    if(timer_settime(timerid, 0, &itv, NULL) == -1){
        perror("timer_settime");
        exit(EXIT_FAILURE);          
    }

    while(pass != 1){
        ;
    }

    if(timer_delete(timerid) == -1){
        perror("timer_delete");
        exit(EXIT_FAILURE);       
    }

    puts("Finish");
    exit(EXIT_SUCCESS);
}