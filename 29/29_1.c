
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void *threadFunc(void *arg)
{
    int statusLocal;
    char *s = (char *) arg;
    //statusLocal = pthread_join(pthread_self(), NULL);
    puts("Start child thread");
    // if(statusLocal != 0){
    //     printf("Error pthread_join in thread %d %s\n", statusLocal, strerror(errno));
    // }
    printf("%s", s);
    return (void *) strlen(s);
}

int main()
{
    pthread_t *thread;
    int status, retVal;

    status = pthread_create(&thread, NULL, threadFunc, "Hello world\n");
    if (status != 0){
        printf("Error create thread %d %s\n", status, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("Message from main()\n");
    //sleep(5);
    //status = pthread_join(pthread_self(), NULL);
    status = pthread_join(thread, &retVal);
    if (status != 0){
        printf("Error pthread_join in main thread %d %s\n", status, strerror(errno));
        //exit(EXIT_FAILURE);
    }
    printf("Thread returned %d\n", retVal);
    exit(EXIT_SUCCESS);

}