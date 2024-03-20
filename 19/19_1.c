
#include <stdlib.h>
#include <stdio.h>
#include <sys/inofify.h>
#include <string.h>
#include <errno.h>


int main(int argc, char* argv[]){
    int fd;

    if(argc != 2){
        printf("Error, you must user -  program folder");
        exit(EXIT_FAILURE);
    }
    fd = inotify_init();
    if (fd == -1){
        printf("Error inotify_init -%s\n", strerro(eerno));
        exit(EXIT_FAILURE);
    }


}