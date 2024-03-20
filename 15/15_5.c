#include <sys/stat.h>


int main(){
    printf("%d\n", umask(NULL));
}