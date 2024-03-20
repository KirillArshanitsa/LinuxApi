#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

extern char **environ;

int my_unsetenv(const char *name){
    //return putenv(name);
    if(getenv(name) != NULL){
        //TODO move elemet in array
        for(char **ep = environ; *ep != NULL; ep++){
            if(strncmp(*ep, name, strlen(name)) == 0){
                strcpy(*ep, " ");
            }
        }
    }
    return 0;
}

int my_setenv(const char *name, const char *value, int overwrite){
    char *str = malloc(sizeof(name) + sizeof(value) + 1);
    strcpy(str, name);
    strcat(str, "=");
    strcat(str, value);

    if(getenv(name) == NULL){
        if(putenv(str) != 0){
            perror("Error putenv");
            return -1;
        }
    }
    else{
        if(overwrite){
            while (getenv(name) != NULL)
            {
                if(putenv(str) != 0){
                    perror("Error putenv");
                    return -1;
                }
            }       
        }       
    }
    return 0;
}



int main(){

    puts("\nPuts elem Test1 Test1");
    my_setenv("Test1", "11", 1);
    my_setenv("Test2", "22", 1);
    for(char **ep = environ; *ep != NULL; ep++){
        puts(*ep);
    }
    puts("\nUnset elem Test1");
    printf("Result my_unsetenv = %d\n", my_unsetenv("Test1"));
    for(char **ep = environ; *ep != NULL; ep++){
        puts(*ep);
    }

}