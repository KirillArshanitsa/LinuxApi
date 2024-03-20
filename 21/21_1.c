#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void my_abort(){
    sigset_t oldSet;
    int isMemberSIGABRT;

    if(sigpending(&oldSet) == -1){
        printf("Error sigpending %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    isMemberSIGABRT = sigismember(&oldSet, SIGABRT);

    if(isMemberSIGABRT){
        if(sigdelset(&oldSet, SIGABRT) == -1){
            printf("Error sigdelset SIGABRT %s\n", strerror(errno));
            exit(EXIT_FAILURE);           
        }

        if(sigprocmask(SIG_SETMASK, &oldSet, NULL) == -1){
             printf("Error sigdelset SIGABRT %s\n", strerror(errno));
            exit(EXIT_FAILURE);               
        }
    }
    else if(isMemberSIGABRT == -1){
        printf("Error sigismmber %s\n", strerror(errno));
        exit(EXIT_FAILURE);       
    }

    if(signal(SIGABRT, SIG_DFL) == SIG_ERR){
        printf("Error signal SIG_DFL %s\n", strerror(errno));
        exit(EXIT_FAILURE);         
    }

    if (fflush(NULL) == EOF){
        printf("Error fflush %s\n", strerror(errno));
        exit(EXIT_FAILURE);        
    }


    if(raise(SIGABRT) != 0){
        printf("Error SIGABRT signal %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}


int main(){
    puts("Start");
    my_abort();
    puts("Finish");
    exit(EXIT_SUCCESS);
}