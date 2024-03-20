#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(){
    int fd1=1, fd2=2;
    int seekFile1, seekFile2;
    int flagsFile1, flagsFile2;
    
    flagsFile1 = fcntl(fd1, F_GETFL);
    flagsFile2 = fcntl(fd2, F_GETFL);

    if ((flagsFile1 == -1) || (flagsFile2 == -1)){
        perror("Error get info fcntl ");
        exit(EXIT_FAILURE);
    }
    if (flagsFile1 == flagsFile2){
        puts("Flags equals");
    }
    else{
        puts("Flags not equals");
    }

    seekFile1 = lseek(fd1, 0, SEEK_CUR);
    seekFile2 = lseek(fd2, 0, SEEK_CUR);

    if ((seekFile1 == -1) || (seekFile2 == -1)){
        perror("Error get info lsseek ");
        exit(EXIT_FAILURE);
    }

    if(seekFile1 == seekFile2){
        puts("Lseek is equals");
    }
    else{
        puts("Lseek is not equals");
    }

}