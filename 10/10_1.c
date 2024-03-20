#include <stdio.h> 
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>

int main(){
    struct tms * buf;
    buf = malloc(sizeof(struct tms));
    printf("sysconf(_SC_CLK_TCK)=%ld  CLOCKS_PER_SEC=%ld\n", sysconf(_SC_CLK_TCK), (long)CLOCKS_PER_SEC);
    for(int i = 10000000; i > 0; i--){
        getpid();
    }
    printf("times = %.2f clock = %.2f\n", (double)times(buf) / sysconf(_SC_CLK_TCK), (double) clock()/CLOCKS_PER_SEC);
    free(buf);

}