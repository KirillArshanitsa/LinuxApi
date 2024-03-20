#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(){
    pid_t child, parrent;
    int wait_status;
    setbuf(stdout, NULL);

    parrent = fork();
    switch (parrent)
    {
    case -1:
        perror("Error create parrent");
        exit(EXIT_FAILURE);
    case 0:
        printf("Start Parrent pid %d\n", getpid());
        child = fork();
        if(child == -1){
            perror("Error create child");
            exit(EXIT_FAILURE);            
        }
        else if(child == 0){
            printf("Before sleep - Chaild pid %d, parrent pid %d\n", getpid(), getppid());
            sleep(7);
            printf("After sleep - Chaild pid %d, parrent pid %d\n", getpid(), getppid());
            sleep(7);
            printf("After sleep wait - Chaild pid %d, parrent pid %d\n", getpid(), getppid());      
            sleep(4);
            printf("After sleep wait 2 - Chaild pid %d, parrent pid %d\n", getpid(), getppid());      
            _exit(EXIT_SUCCESS);
        }
        else{
            sleep(2);
            printf("Finis parrent pid %d\n", getpid());
            _exit(EXIT_SUCCESS);
        }
        
    default:
        sleep(6);
        printf("Ancestor process before wait %d\n", getpid());
        sleep(10);
        wait_status = wait(NULL);
        printf("Ancestor process after wait %d status - %d\n", getpid(), wait_status);
        sleep(8);
        exit(EXIT_SUCCESS);
    }

}