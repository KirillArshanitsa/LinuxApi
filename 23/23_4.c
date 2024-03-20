/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2023.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TIMER_SIG SIGRTMAX              /* Our timer notification signal */
#define BUF_SIZE 256



void itimerspecFromStr(char *str, struct itimerspec *tsp)
{
    char *dupstr ,*cptr, *sptr;

    dupstr = strdup(str);

    cptr = strchr(dupstr, ':');
    if (cptr != NULL)
        *cptr = '\0';

    sptr = strchr(dupstr, '/');
    if (sptr != NULL)
        *sptr = '\0';

    tsp->it_value.tv_sec = atoi(dupstr);
    tsp->it_value.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;

    if (cptr == NULL) {
        tsp->it_interval.tv_sec = 0;
        tsp->it_interval.tv_nsec = 0;
    } else {
        sptr = strchr(cptr + 1, '/');
        if (sptr != NULL)
            *sptr = '\0';
        tsp->it_interval.tv_sec = atoi(cptr + 1);
        tsp->it_interval.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;
    }
    free(dupstr);
}

char * currTime(const char *format)
{
    static char buf[BUF_SIZE];  /* Nonreentrant */
    time_t t;
    size_t s;
    struct tm *tm;

    t = time(NULL);
    tm = localtime(&t);
    if (tm == NULL)
        return NULL;

    s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);

    return (s == 0) ? NULL : buf;
}


int
main(int argc, char *argv[])
{
    sigset_t set;
    siginfo_t info;
    struct itimerspec ts;
//    struct sigaction  sa;
    struct sigevent   sev;
    timer_t *tidlist;
    int j, sig;

    if (argc < 2)
        printf("%s secs[/nsecs][:int-secs[/int-nsecs]]...\n", argv[0]);

    tidlist = calloc(argc - 1, sizeof(timer_t));
    if (tidlist == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    /* Establish handler for notification signal */

    // sa.sa_flags = SA_SIGINFO;
    // sa.sa_sigaction = handler;
    // sigemptyset(&sa.sa_mask);
    // if (sigaction(TIMER_SIG, &sa, NULL) == -1){
    //     perror("sigaction");
    //     exit(EXIT_FAILURE);
    // }

    /* Create and start one timer for each command-line argument */

    sev.sigev_notify = SIGEV_SIGNAL;    /* Notify via signal */
    sev.sigev_signo = TIMER_SIG;        /* Notify using this signal */

    sigemptyset(&set);
    sigaddset(&set, TIMER_SIG);

    if(sigprogmask(SIG_BLOCK, &set, NULL) ==- 1){
        perror("sigprogmask");
        exit(EXIT_FAILURE);
    }

    for (j = 0; j < argc - 1; j++) {
        itimerspecFromStr(argv[j + 1], &ts);

        sev.sigev_value.sival_ptr = &tidlist[j];
                /* Allows handler to get ID of this timer */

        if (timer_create(CLOCK_REALTIME, &sev, &tidlist[j]) == -1){
            perror("timer_create");
            exit(EXIT_FAILURE);
        }
        printf("Timer ID: %ld (%s)\n", (long) tidlist[j], argv[j + 1]);

        if (timer_settime(tidlist[j], 0, &ts, NULL) == -1){
            perror("timer_settime");
            exit(EXIT_FAILURE);
        }
    }

    for (;;){
        sig = sigwaitinfo(&set, &info);
        printf("[%s] Got signal %d\n", currTime("%T"), sig);
        printf("    *sival_ptr         = %ld\n", (long) info.si_value.sival_ptr);
        printf("    timer_getoverrun() = %d\n", timer_getoverrun(info.si_value.sival_ptr));
    }                   
        
}