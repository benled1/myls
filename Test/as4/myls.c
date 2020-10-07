#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <getopt.h>
#include <pwd.h>
#include <grp.h>
#include "mylsfunctions.h"

//structs to store the directory info inside of
struct stat sfile;
struct stat testfile;
struct stat lengthfile;
struct dirent * pDirent;
DIR * pDirectory;



int main(int numArgs, char ** args) {
    
    //int in order to getopt() the args
    int arguType;
    //count of the files in the current dir
    int count;
    //struct used to sort the fileNames and hold info on them
    struct array_name *fileNames;
    
    //arguments to indicate which options have been chosen
    int argHasi = 0;
    int argHasl = 0;
    int argHasR = 0;
    
    //filepaths to call open directory on in order to see inside the directory
    char * filepath = malloc(1500*sizeof(char));
    char * testpath = malloc(1500*sizeof(char));
    
    //indicates the index in main command line args where the options end
    int dirStart = numArgs;

    //indicates whether or not we printed using argR when there are no dir arguments
    int dontcontinue = 0;

    //struct to hold the info about the max lengths of each field in -i and -l
    struct lengths len;

    //used to find the lengths of each aspect about a single file
    int fileIno;
    int fileUser;
    int fileGroup;
    int fileSize;
    int fileLink;

    //used to help find the lengths of user and group ID about a single file
    struct passwd *uID;
    struct group *groupID;

    
    

    //find the index where the directories start and the options end
    for(int i = 1; i < numArgs;i++)
    {
        if(args[i][0] != '-')
        {
            dirStart = i;
            break;
        }
    }

    //find which options have been chosen
    while((arguType = getopt(dirStart,args,"ilR"))!=-1)
    {
        switch(arguType)
        {
            case 'i':
                argHasi = 1;
                break;
            case 'l':
                argHasl = 1;
                break;
            case 'R':
                argHasR = 1;
                break;
            default:
                printf("There were no args given");
        }
    }
    
    //if there are given dir and file args loop through all args given
    for(int j = dirStart;j<numArgs;j++)
    {
        
        
        strcpy(testpath,args[j]);
        memset(&testfile,0,sizeof testfile);
        
        //check if a valid file by calling stat
        if(lstat(testpath, &testfile)!=0)
        {
            perror("Failed");
        }
        //find the widths of all the fields about the file
        fileIno = findDigits(testfile.st_ino);
        fileSize = findDigits(testfile.st_size);
        fileLink = findDigits(testfile.st_nlink);
        uID = getpwuid(testfile.st_uid);
        groupID = getgrgid(testfile.st_gid);
        fileUser = strlen(uID->pw_name);
        fileGroup = strlen(groupID->gr_name);

        //if the file is not a directory print out the correct fields
        if((S_ISDIR(testfile.st_mode)==0) && (S_ISREG(testfile.st_mode)!=0))
        {
            printf("\n'%s':\n",args[j]);
            if(argHasi == 1)
            {
                argumentI(&testfile,fileIno);
            }
            if(argHasl == 1)
            {
                argumentL(&testfile, fileUser,fileGroup,fileSize,fileLink);
            }
            printf("%s \n", args[j]);
            //skip the rest of the loop as that pertains to directories only
            continue;
        }
     
            
        //assuming we are here then the arg given is a directory
        //open up the directory and count the number of files in it
        pDirectory = opendir(args[j]);
        if(pDirectory == NULL) {
            printf("Cannot open %s\n", args[j]);
            return -1;
        }
        count = countFiles(pDirectory);
        fileNames = (struct array_name*) malloc(count*sizeof(struct array_name));
        closedir(pDirectory);

        //re-open the dir and sort the files in it
        pDirectory = opendir(args[j]);
        if(pDirectory == NULL) {
            printf("Cannot open %s\n", args[j]);
            return -1;
        }
        //sort the files
        fileNames = sort_fileNames(fileNames,count,pDirectory,args[j]);
        closedir(pDirectory);
        
        //finally open it once more 
        pDirectory = opendir(args[j]);
        if(pDirectory == NULL) {
            printf("Cannot open %s\n", args[j]);
            return -1;
        }
        

        //check if -R is an option
        //if so then call the argumentR() function which handles the recursion and printing
        if(argHasR == 1)
        {
            strcpy(filepath,args[j]);
            argumentR(fileNames,count,filepath, argHasi,argHasl);
            continue;
        }
        
        //if you are here then it was not the -R option
        //find the max widths of all the fields in each file in the directory
        len = maxLengths(args[j],count,fileNames);

        //loop through all files in the directory
        for(int i = 0;i < count;i++)
        {
            //call stat on each file in the order that they were sorted into fileNames struct
            strcpy(filepath,args[j]);
            filepath = strcat(filepath,"/");
            filepath = strcat(filepath,fileNames[i].name);

            if(lstat(filepath, &sfile)!=0)
            {
                
                perror("stat failed");
            }
            
            if(argHasi == 1)
            {
                argumentI(&sfile,len.ino);
            }
            if(argHasl == 1)
            {
                argumentL(&sfile,len.max_user,len.max_group,len.size, len.link);
            }
            argumentNone(i,fileNames);
            
        }
        closedir(pDirectory);
    }

    
    //if you enter this if then that means no file/dir args were supplied
    //this performs the above operations but only on the single "." directory
    if(dirStart == numArgs)
    {
        
        pDirectory = opendir(".");
        if(pDirectory == NULL) {
            printf("Cannot open %s\n", ".");
            return -1;
        }
        count = countFiles(pDirectory);
        fileNames = (struct array_name*) malloc(count*sizeof(struct array_name));
        closedir(pDirectory);

        pDirectory = opendir(".");
        if(pDirectory == NULL) {
            printf("Cannot open %s\n",".");
            return -1;
        }

        fileNames = sort_fileNames(fileNames,count,pDirectory,".");
        closedir(pDirectory);

        if(argHasR == 1)
        {
            strcpy(filepath,".");
            argumentR(fileNames,count,filepath, argHasi,argHasl);
            dontcontinue = 1;
        }

        len = maxLengths(".",count,fileNames);
        //if you already called argumentR() don't continue
        if(dontcontinue != 1)
        {
            for(int i = 0;i < count;i++)
            {
                pDirectory = opendir(".");
                if(pDirectory == NULL) {
                printf("Cannot open %s\n", ".");
                return -1;
                }
                strcpy(filepath,".");
                filepath = strcat(filepath,"/");
                filepath = strcat(filepath,fileNames[i].name);
                
                if(lstat(filepath, &sfile)!=0)
                {
                    
                    perror("stat failed");
                }
                
                if(argHasi == 1)
                {
                    argumentI(&sfile,len.ino);
                }
                if(argHasl == 1)
                {
                    argumentL(&sfile,len.max_user,len.max_group, len.size,len.link);
                }
                argumentNone(i,fileNames);
            }
        }
        
    }
    
    
    free(filepath);
    free(testpath);
    
    

    return 0;
}