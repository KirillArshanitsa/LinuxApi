#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <string.h>

int main(int argc, char *argv[])
{
    int numChildren, j;
    pid_t childPid;

    if (argc > 1 && strcmp(argv[1], "--help") == 0){
        printf("%s [num-children]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numChildren = (argc > 1) ? atoi(argv[1]) : 1;

    setbuf(stdout, NULL);               /* Make stdout unbuffered */
    for (j = 0; j < numChildren; j++) {
        switch (childPid = fork()) {
        case -1:
            puts("fork");
            exit(EXIT_FAILURE);

        case 0:
            printf("%d child\n", j);
            _exit(EXIT_SUCCESS);

        default:
            printf("%d parent\n", j);
            wait(NULL);                 /* Wait for child to terminate */
            break;
        }
    }

    exit(EXIT_SUCCESS);
}