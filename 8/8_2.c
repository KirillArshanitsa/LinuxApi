#include <pwd.h>
#include <string.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

struct passwd *my_getpwnam(const char *name){
    struct passwd *pwd;
    pwd = malloc(sizeof(struct passwd));
    while((pwd = getpwent()) != NULL){
        if (strcmp(pwd->pw_name, name)== 0){
            return pwd;
        }
    }
    endpwent();
    free(pwd);
    return NULL;
}

int main(){
    char username[] ="sbt-arshanitsa-ka"; 
    struct passwd *pwd;
    pwd = my_getpwnam(username);
    if(pwd == NULL){
        printf("User %s not found\n", username);
    }
    else{
        printf("User %s found\n", username);
        printf("%s, %d, %d, %ld, %s, %s, %ld, %s, %s, %s\n", pwd->pw_name, pwd->pw_uid, pwd->pw_gid, pwd->pw_change, pwd->pw_class, pwd->pw_dir, pwd->pw_expire, pwd->pw_gecos, pwd->pw_shell, pwd->pw_passwd);
        free(pwd);
    }
    exit(EXIT_SUCCESS);
}