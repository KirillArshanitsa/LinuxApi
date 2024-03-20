#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

    execve(argv[1], argVec, envVec);
    errExit("execve");   /* Если мы сюда добрались, что-то пошло не так */
}