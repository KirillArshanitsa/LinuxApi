#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define __USE_XOPEN_EXTENDED
#include <ftw.h>


int my_func(const char * pathname, const struct stat * statbuf, int typefLag, struct FTW *ftwbuf){
printf("my_func path - %s, ino = %lu, dev = %lu, size = %lu, typefLag = %d ", pathname, statbuf->st_ino, statbuf->st_dev, statbuf->st_size, typefLag);
printf("typefLag = %s\n", (typefLag == FTW_F) ? "FTW_F" : (typefLag == FTW_D) ? "FTW_D" : (typefLag == FTW_DNR) ? "FTW_DNR" : (typefLag == FTW_DP) ? "FTW_DP" : (typefLag == FTW_NS) ? "FTW_NS" :
(typefLag == FTW_SL) ? "FTW_SL" : (typefLag == FTW_SLN) ? "FTW_SLN" : "Not difine");
return 0;
}


int my_nftw(const char *dirpath, int(*func) (const char * pathname, const struct stat * statbuf, int typefLag, struct FTW *ftwbuf), int nopenfd, int fLags){

DIR *dirPtr;
struct dirent *direntPtr;
unsigned long fileCounts = 0;
unsigned long folderCounts = 0;
struct dirent *direntPtrFiles;
struct dirent *direntPtrFolderd;
unsigned long direntPtrFileSize= 0;
unsigned long direntPtrFolderSize = 0;
struct stat statbuf;
struct stat mainFolderStatBuf;
struct FTW ftwbuf;
char path[PATH_MAX];
int typefLag;


dirPtr = opendir(dirpath);
if (dirPtr == NULL){
printf("Error opendir %s - %s", dirpath, strerror(errno));
return -1;
}

errno = 0;
while ((direntPtr = readdir(dirPtr)) != NULL) {
if (direntPtr->d_type == DT_DIR)
folderCounts++;
else
fileCounts++;
}
if (errno != 0) {
printf("Error direntPtr %s - %s", dirpath, strerror(errno));
return -1;
}


direntPtrFiles = malloc(sizeof(struct dirent) * fileCounts);
direntPtrFolderd = malloc(sizeof(struct dirent) * folderCounts);
if ((direntPtrFiles == NULL) || (direntPtrFolderd == NULL)) {
printf("Error malloc\n");
if(closedir(dirPtr)== -1)
printf("Error closedir %s - %s", dirpath, strerror(errno));
return -1;
}

rewinddir(dirPtr);
while( (direntPtr = readdir(dirPtr)) != NULL) {
if(strcmp(direntPtr->d_name, ".") == 0 | strcmp(direntPtr->d_name, "..") == 0)
continue;
if (direntPtr->d_type == DT_DIR) {
memcpy(&(direntPtrFolderd[direntPtrFolderSize]), direntPtr, sizeof(struct dirent));
direntPtrFolderSize++;
}
else{
memcpy(&(direntPtrFiles[direntPtrFileSize]), direntPtr, sizeof(struct dirent));
direntPtrFileSize++;
}
}


switch (fLags)
{
case FTW_CHDIR:

break;
case FTW_DEPTH:
for(unsigned long folder = 0; folder < direntPtrFolderSize; folder++){
strcpy(path, dirpath);
strcat(path,"/");
strcat(path,direntPtrFolderd[folder].d_name);
if(my_nftw(path, func, nopenfd, fLags) == -1)
return -1;

if(stat(path, &statbuf) == -1){
printf("Error stat folder %s - %s\n", path, strerror(errno));
typefLag = FTW_DNR;
}
else
typefLag = FTW_D;

if(func(path, &statbuf, typefLag, &ftwbuf) == -1)
return -1;
}

for(unsigned long file = 0; file < direntPtrFileSize; file++){
strcpy(path, dirpath);
strcat(path,"/");
strcat(path,direntPtrFiles[file].d_name);
typefLag = FTW_F;
if(stat(path, &statbuf) == -1){
printf("Error stat file %s - %s\n", path, strerror(errno));
typefLag = FTW_NS;
}

if(func(path, &statbuf, typefLag, &ftwbuf) == -1)
return -1;
}

if (errno != 0) {
printf("Error direntPtr %s - %s", dirpath, strerror(errno));
return -1;
}
break;
case FTW_MOUNT:
if(stat(dirpath, &mainFolderStatBuf) == -1){
printf("Error stat folder %s - %s\n", dirpath, strerror(errno));
return -1;
}
for(unsigned long folder = 0; folder < direntPtrFolderSize; folder++){
strcpy(path, dirpath);
strcat(path,"/");
strcat(path,direntPtrFolderd[folder].d_name);

if(stat(path, &statbuf) == -1){
printf("Error stat folder %s - %s\n", path, strerror(errno));
typefLag = FTW_DNR;
}
else
typefLag = FTW_D;

if( (mainFolderStatBuf.st_dev != statbuf.st_dev) || (mainFolderStatBuf.st_ino != statbuf.st_ino) ){
return 0;
}

if(my_nftw(path, func, nopenfd, fLags) == -1)
return -1;

if(func(path, &statbuf, typefLag, &ftwbuf) == -1)
return -1;
}

for(unsigned long file = 0; file < direntPtrFileSize; file++){
strcpy(path, dirpath);
strcat(path,"/");
strcat(path,direntPtrFiles[file].d_name);
typefLag = FTW_F;
if(stat(path, &statbuf) == -1){
printf("Error stat file %s - %s\n", path, strerror(errno));
typefLag = FTW_NS;
}

if( (mainFolderStatBuf.st_dev != statbuf.st_dev) || (mainFolderStatBuf.st_ino != statbuf.st_ino) ){
return 0;
}

if(func(path, &statbuf, typefLag, &ftwbuf) == -1)
return -1;
}

if (errno != 0) {
printf("Error direntPtr %s - %s", dirpath, strerror(errno));
return -1;
}
break;
case FTW_PHYS:
for(unsigned long folder = 0; folder < direntPtrFolderSize; folder++){
strcpy(path, dirpath);
strcat(path,"/");
strcat(path,direntPtrFolderd[folder].d_name);
if(my_nftw(path, func, nopenfd, fLags) == -1)
return -1;

if(lstat(path, &statbuf) == -1){
printf("Error stat folder %s - %s\n", path, strerror(errno));
typefLag = FTW_DNR;
}
else
typefLag = FTW_D;

if(func(path, &statbuf, typefLag, &ftwbuf) == -1)
return -1;
}

for(unsigned long file = 0; file < direntPtrFileSize; file++){
strcpy(path, dirpath);
strcat(path,"/");
strcat(path,direntPtrFiles[file].d_name);
typefLag = FTW_F;
if(lstat(path, &statbuf) == -1){
printf("Error stat file %s - %s\n", path, strerror(errno));
typefLag = FTW_NS;
}
if((statbuf.st_mode & S_IFMT) == S_IFLNK)
typefLag = FTW_SL;
if(func(path, &statbuf, typefLag, &ftwbuf) == -1)
return -1;
}

if (errno != 0) {
printf("Error direntPtr %s - %s", dirpath, strerror(errno));
return -1;
}
break;
default:
printf("Error fLags = %d\n", fLags);
exit(EXIT_FAILURE);
}


free(direntPtrFiles);
free(direntPtrFolderd);
if(closedir(dirPtr)== -1){
printf("Error closedir %s - %s", dirpath, strerror(errno));
return -1;
}
return 0;
}



int main(){
printf("Result = %d\n", my_nftw("/Users/sbt-arshanitsa-ka-mobile/Documents/C/LinuxApi",my_func,1,FTW_DEPTH));
}