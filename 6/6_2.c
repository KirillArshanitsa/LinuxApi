#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env;

void myFuns(){
    if(setjmp(env) == 0){
        puts("setjmp == 0");
    }
    else{
        puts("setjmp == 1");
    }
}


int main(){
    puts("Start run function");
    myFuns();
    puts("Return from function");
    puts("Strat jump");
    longjmp(env, 1);
    puts("Exit program");
    exit(EXIT_SUCCESS);
}