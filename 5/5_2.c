#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    int fd;
    char begin_str[] = "Before append in my_file_5_2.txt";
    char finish_str[] = "After seek write to file";

    fd = open("my_file_5_2.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd == -1){
        perror("Error crete file my_file_5_2.txt");
        exit(EXIT_FAILURE);
    }

    if (write(fd, begin_str, sizeof(begin_str)) == -1){
        perror("Error write first string in file my_file_5_2.txt");
        exit(EXIT_FAILURE);       
    }

    if (close(fd) == -1){
        perror("Error close file my_file_5_2.txt");
        exit(EXIT_FAILURE);           
    }



    fd = open("my_file_5_2.txt", O_WRONLY | O_APPEND);
    if(fd == -1){
        perror("Error open for write file my_file_5_2.txt");
        exit(EXIT_FAILURE);
    } 
    if (lseek(fd, 0, SEEK_SET) == -1){
        perror("Error seek in begin file file my_file_5_2.txt");
        exit(EXIT_FAILURE);       
    } 
    if (write(fd, finish_str, sizeof(finish_str)) == -1){
        perror("Error write string after seek in file my_file_5_2.txt");
        exit(EXIT_FAILURE);       
    }


    if (close(fd) == -1){
        perror("Error close file my_file_5_2.txt");
        exit(EXIT_FAILURE);           
    }

    exit(EXIT_SUCCESS); 

}