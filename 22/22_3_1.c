/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2023.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* sig_speed_sigsuspend.c

   This program times how fast signals are sent and received.

   The program forks to create a parent and a child process that alternately
   send signals to each other (the child starts first). Each process catches
   the signal with a handler, and waits for signals to arrive using
   sigsuspend().

   Usage: $ time ./sig_speed_sigsuspend num-sigs

   The 'num-sigs' argument specifies how many times the parent and
   child send signals to each other.

   Child                                  Parent

   for (s = 0; s < numSigs; s++) {        for (s = 0; s < numSigs; s++) {
       send signal to parent                  wait for signal from child
       wait for a signal from parent          send a signal to child
   }                                      }
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>


static void
handler(int sig)
{
    printf("signal %d %s\n", sig, strsignal(sig));
}

#define TESTSIG SIGUSR1

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        printf("%s num-sigs\n", argv[0]);

    int numSigs = atoi(argv[1]);

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(TESTSIG, &sa, NULL) == -1){
        puts("sigaction");
        exit(EXIT_FAILURE);
    }

    /* Block the signal before fork(), so that the child doesn't manage
       to send it to the parent before the parent is ready to catch it */

    sigset_t blockedMask, emptyMask;
    sigemptyset(&blockedMask);
    sigaddset(&blockedMask, TESTSIG);
    if (sigprocmask(SIG_SETMASK, &blockedMask, NULL) == -1){
        puts("sigprocmask");
        exit(EXIT_FAILURE);
    }
    sigfillset(&emptyMask);
    int scnt, scnt2;
    pid_t childPid = fork();
    switch (childPid) {
    case -1: 
        puts("fork");
        exit(EXIT_FAILURE);
    case 0:     /* child */
        for (scnt = 0; scnt < numSigs; scnt++) {
            if (kill(getppid(), TESTSIG) == -1){
                puts("kill");
                exit(EXIT_FAILURE);
            }
            if (sigwaitinfo(&emptyMask, NULL) == -1){
                puts("sigwaitinfo");
                exit(EXIT_FAILURE);
            }
        }
        exit(EXIT_SUCCESS);

    default: /* parent */
        for (scnt2 = 0; scnt2 < numSigs; scnt2++) {
            if (sigwaitinfo(&emptyMask, NULL) == -1){
                puts("sigwaitinfo");
                exit(EXIT_FAILURE);               
            }
            if (kill(childPid, TESTSIG) == -1){
                puts("kill");
                exit(EXIT_FAILURE);
            }
        }
        exit(EXIT_SUCCESS);
    }
}