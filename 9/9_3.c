#define _BSD_SOURCE
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
//#include <sys/types.h>

int my_initgroups(const char * user, gid_t group){
    struct passwd * pwd;
    struct group * grp;
    size_t gidSize = 0;
    long listMaxSize  = sysconf(_SC_NGROUPS_MAX);
    gid_t groupList[listMaxSize];

    pwd = getpwnam(user);
    if(pwd == NULL){
        printf("User %s not found\n", user);
        return -1;
    }

    while( (grp = getgrent()) != NULL ){
        for(int i = 0; grp->gr_mem[i] != NULL; i ++){
            if(strcmp(user, grp->gr_mem[i]) == 0){
                groupList[gidSize] = grp->gr_gid;
                gidSize++;
            }
        }
    }
    groupList[gidSize] = group;
    gidSize++;

    return setgroups(gidSize, groupList);
}

int main(){
    char user[] ="ansible";
    gid_t group = 70;
    int groupCount = 0;
    gid_t groupList[sysconf(_SC_NGROUPS_MAX)];

    groupCount = getgroups(0, groupList);
    printf("Groups cont before = %d\n", groupCount);
    getgroups(groupCount, groupList);
    for(int i = groupCount - 1; i >= 0; i--){
        printf("%d ", (long)groupList[i]);
    }
    puts("");

    if(my_initgroups(user, group) == -1){
        printf("Error my_initgroups for user %s and group %ld\n", user, (long)group);
        exit(EXIT_FAILURE);
    }
    else{
        groupCount = getgroups(0, "");
        printf("Groups cont after = %d\n", groupCount);
        getgroups(groupCount, groupList);
        for(int i = groupCount -1 ; i >= 0; i--){
            printf("%d ", (long)groupList[i]);
        }
        puts("");
        exit(EXIT_FAILURE);
    }
}