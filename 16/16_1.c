#include <sys/xattr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]){
    char * key;
    if (argc != 4){
        printf("You must user program filename key value\n");
        exit(EXIT_FAILURE);
    }
    key = malloc(strlen(argv[2]) + 5);//user.x
    strcpy(key, "user.x");
    strcat(key, argv[2]);

    if (setxattr(argv[1], argv[2], argv[3], strlen(argv[3]), 0) == -1){
        printf("Error setxattr %s %s for file %s\n", argv[2], argv[3], argv[1]);
        free(key);
        exit(EXIT_FAILURE);       
    }
        free(key);
        exit(EXIT_SUCCESS); 

}