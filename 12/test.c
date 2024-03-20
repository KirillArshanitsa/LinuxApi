#include <stdio.h>
#include <unistd.h>

int main(){
    printf("printf");
    write(STDOUT_FILENO, "write", 6);
}