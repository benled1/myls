#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#ifndef LS_FUNC
#define LS_FUNC
//struct for sorting
//Testing for Sorting
struct array_name {    

    //file_name
    char name[10000];
    
    //int userID and groupID
    char * userID;
    char * groupID;

    //time file
    time_t time;

    //permission file mode
    __mode_t s_mode;

    //size of the file
    long size_file;

    __ino_t i_node;

    long link;

};

struct lengths{
    int ino;
    int size;
    int max_user;
    int max_group;
    int link;
};

//struct stat sfile;
//struct dirent * pDirent;
//struct array_name *fileNames;
//DIR * pDirectory;

//prints the correct information from the arg -i
void argumentI(struct stat * statFile, int maxIno);

//does nothing right now
char* checkArg(char *myarg,int count);

//prints the correct information from the arg -l
void *argumentL(struct stat *buffer, int maxUser, int maxGroup,int maxSize,int maxLink);

//prints the correct information from the arg -R
void argumentR(struct array_name *sortedNames,int count,char * passedfilepath,
                int argHasi, int argHasl);

//prints the correct information for no arguments
void argumentNone(int index,struct array_name *fileNames);

//counts the number of files in the given directory
int countFiles(DIR * pDirectory);

//sort the filenames in lexo
struct array_name *sort_fileNames(struct array_name *fileNames, int length,
                                    DIR *pDirectory,char * path);

//find max length of columns of info
struct lengths maxLengths(char * argument, int count, struct array_name *fileNames);

int findDigits(int Number);




#endif