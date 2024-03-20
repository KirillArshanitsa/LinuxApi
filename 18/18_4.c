
/* list_files.c

   Demonstrate the use of opendir() and related functions to list files
   in a directory.

   Walk through each directory named on the command line (current directory
   if none are specified) to display a list of the files it contains.

    Usage: list_files [dir...]
*/
#if defined(__APPLE__)
        /* Darwin requires this header before including <dirent.h> */
#include <sys/types.h>
#endif
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void             /* List all files in directory 'dirpath' */
listFiles(const char *dirpath)
{
    DIR *dirp;
    struct dirent *dp;
    int isCurrent;          /* True if 'dirpath' is "." */
    struct dirent **result;

    isCurrent = strcmp(dirpath, ".") == 0;

    dirp = opendir(dirpath);
    if (dirp  == NULL) {
        printf("opendir failed on '%s'", dirpath);
        return;
    }

    /* For each entry in this directory, print directory + filename */

    for (;;) {
        errno = 0;              /* To distinguish error from end-of-directory */
        dp = malloc(sizeof(struct dirent));
       
        if (readdir_r(dirp, dp, result) != 0){
            printf("Error readdir_r\n"); 
             exit(EXIT_FAILURE);
        }
        if (dp == NULL)
            break;

        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;           /* Skip . and .. */

        if (!isCurrent)
            printf("%s/", dirpath);
        printf("%s\n", dp->d_name);
        free(dp);
    }

    if (errno != 0){
        printf("readdir");
        exit(EXIT_FAILURE);
    }

    if (closedir(dirp) == -1)
        printf("closedir");


    //puts((*result)->d_name );
    // while(result != NULL){
    //     puts((*result)->d_name );  
    //     free(*result);
    //     result++;
    // }
    
}

int
main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0){
        printf("%s [dir-path...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc == 1)              /* No arguments - use current directory */
        listFiles(".");
    else
        for (argv++; *argv; argv++)
            listFiles(*argv);

    exit(EXIT_SUCCESS);
}