
#include <ftw.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

struct FolderData
{
    unsigned long REG;
    unsigned long DIR;
    unsigned long CHR;
    unsigned long BLK;
    unsigned long FIFO;
    unsigned long SOCK;
    unsigned long LNK;

};

struct FolderData myData;

int getFsInfo(const char * pathname, const struct stat * statbuf, int typefLag, struct FTW *ftwbuf){

    switch (statbuf->st_mode & S_IFMT)
    {
    case  S_IFREG:
        myData.REG++;
        break;
     case  S_IFDIR:
        myData.DIR++;
        break;
    case  S_IFCHR:
        myData.CHR++;
        break;
    case  S_IFBLK:
        myData.BLK++;
        break;
    case  S_IFLNK:
        myData.LNK++;
        break;
    case  S_IFIFO:
        myData.FIFO++;
        break;
    case  S_IFSOCK:
        myData.SOCK++;
        break;                                           
    default:
        break;
    }

    return 0;
}






int main(){
    double allData;
    nftw("/Users/sbt-arshanitsa-ka-mobile/Documents/C/LinuxApi", getFsInfo, 50, 0);
    allData = myData.REG + myData.DIR + myData.CHR + myData.BLK + myData.LNK + myData.FIFO + myData.SOCK;
    printf("REG=%lu %%=%f \nDIR=%lu %%=%f \nCHR=%lu %%=%f\nBLK=%lu %%=%f\nLNK=%lu %%=%f\nFIFO=%lu %%=%f\nSOCK=%lu %%=%f\n", myData.REG, (myData.REG/allData) * 100, myData.DIR, (myData.DIR / allData) * 100,
     myData.BLK, (myData.BLK / allData) * 100, myData.LNK, (myData.LNK / allData) * 100, myData.FIFO, (myData.FIFO/allData) * 100, myData.SOCK, (myData.SOCK/ allData)* 100);
}