#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <getopt.h>
#include <pwd.h>
#include <grp.h>
#include <math.h>
#include "mylsfunctions.h"

//For UserID AND GroupID
struct passwd *userID;
struct group *groupID;

//Convert from st_mtime to proper time 
struct tm timeStruct;
char mtime[100];

//find the number of digits in a given number
int findDigits(int Number)
{
    int Length;
        if(Number <= 9999999999)
        {
            if(Number <= 999999999)
            {
                if(Number <= 99999999)
                {
                    if(Number <= 9999999)
                    {
                        if(Number <= 999999)
                        {
                            if(Number <= 99999)
                            {
                                if(Number <= 9999)
                                {
                                    if(Number <= 999)
                                    {
                                        if(Number <= 99)
                                        {
                                            if(Number <= 9)
                                            {
                                                
                                                Length = 1;
                                                return Length;
                                            }
                                            Length = 2;
                                            return Length;
                                        }
                                        Length = 3;
                                        return Length;
                                    }
                                    Length = 4;
                                    return Length;
                                }
                                Length = 5;
                                return Length;
                            }
                            Length = 6;
                            return Length;
                        }
                        Length = 7;
                    return Length;
                    }
                    Length = 8;
                    return Length;
                }
                Length = 9;
                return Length;
            }
            Length = 10;
            return Length;
        }
        Length = 11;
        return Length;
}


//find the maxLengths of all fields and then return a struct containing them
struct lengths maxLengths(char * argument, int count, struct array_name *fileNames)
{
    //initialize all variables
    int inoLength = 0;
    int maxIno = 0;
    int sizeLength = 0;
    int maxSize = 0;
    int linkLength = 0;
    int maxLink = 0;
    
    int inoNumber;
    long sizeNumber;
    int linkNumber;
    
    struct stat lengthfile;
    char * lengthpath = malloc(1500*sizeof(char));
    
    int maxlenUser = 0;
    int maxlenGroup = 0;


    struct lengths len;
    for(int k =0;k<count;k++)
    {
        //SET UP
        strcpy(lengthpath,argument);
        lengthpath = strcat(lengthpath,"/");
        lengthpath = strcat(lengthpath,fileNames[k].name);
        inoLength = 1;
        
        if(lstat(lengthpath, &lengthfile)!=0)
        {
            printf("before perror");
            perror("stat failed");
        }

        //FIND THE LENGTH OF THE INO 
        inoNumber = lengthfile.st_ino;
        while(inoNumber != 0)
        {
            inoLength++;

            inoNumber = inoNumber / 10;
        }
        if(inoLength > maxIno)
        {
            
            maxIno = inoLength;
        }


        //FIND THE LENGTH OF THE NAMES OF USER AND GROUP
        userID = getpwuid(lengthfile.st_uid);
        groupID = getgrgid(lengthfile.st_gid);

        len.max_user = strlen(userID->pw_name);
        len.max_group = strlen(groupID->gr_name);

        if(maxlenUser < len.max_user)
        {
            maxlenUser = len.max_user;
        }
        if(maxlenGroup < len.max_group)
        {
            maxlenGroup = len.max_group;
        }

        
        
        
        //FIND THE MAX LENGTH OF THE SIZE
        sizeNumber = lengthfile.st_size;
        sizeLength = findDigits(sizeNumber);
        if(sizeLength > maxSize)
        {
            
            maxSize = sizeLength;
 
        }
       
        //FIND THE LENGTH OF THE LINK
        linkNumber = lengthfile.st_nlink;
        linkLength = findDigits(linkNumber);
        if(linkLength > maxLink)
        {
            maxLink = linkLength;
        
        }
        
        




    }
    //assign the max lengths to the struct
    len.link = maxLink;
    len.size = maxSize;
    len.max_user = maxlenUser;
    len.max_group = maxlenGroup;
    len.ino = maxIno;
    free(lengthpath);
    return len;
    
}

//this function prints the name of the file/dir
void argumentNone(int index,struct array_name *fileNames)
{   
    
    printf("%-16s\n",fileNames[index].name);
}

//prints out the properly formatted -i command and nothing else
void argumentI(struct stat * statFile, int maxIno)
{
    printf("%*ld ", maxIno,statFile->st_ino);
}

//prints out the properly formatted -l command and nothing else
void *argumentL(struct stat *buffer,int maxUser,int maxGroup, int maxSize,int maxLink)
{
    //obtain the user and group iD
    userID = getpwuid(buffer->st_uid);
    groupID = getgrgid(buffer->st_gid);

    //print out the different permissions
    printf((S_ISDIR(buffer->st_mode)) ? "d" : "-");
    printf((buffer->st_mode & S_IRUSR) ? "r" : "-");
    printf((buffer->st_mode & S_IWUSR) ? "w" : "-");
    printf((buffer->st_mode & S_IXUSR) ? "x" : "-");
    printf((buffer->st_mode & S_IRGRP) ? "r" : "-");
    printf((buffer->st_mode & S_IWGRP) ? "w" : "-");
    printf((buffer->st_mode & S_IXGRP) ? "x" : "-");
    printf((buffer->st_mode & S_IROTH) ? "r" : "-");
    printf((buffer->st_mode & S_IWOTH) ? "w" : "-");
    printf((buffer->st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");

    //print the link userid and groupid
    printf("%*ld ", maxLink,buffer->st_nlink);
    printf("%*s ", maxUser,userID->pw_name);
    printf("%*s ", maxGroup,groupID->gr_name);

    printf("%*ld ", maxSize,buffer->st_size);

    //Time conversion FOR TIME
    time_t t = buffer->st_mtime;
    localtime_r(&t,&timeStruct);
    //print the time
    strftime(mtime, sizeof mtime, "%b %-2d %y%y %H:%M", &timeStruct);
    printf("%s ", mtime);

    return NULL;
}

//COUNT FILES IN THE DIRECTORY
int countFiles(DIR * pDirectory)
{
    struct dirent *pDirent;
    int count = 0;
    while((pDirent = readdir(pDirectory))!= NULL ) {
        //dont count the dots
        if(strcmp(pDirent->d_name,".") == 0 || strcmp(pDirent->d_name,"..") == 0) {
            count += 0;
            continue;
        }
        
        else {
            count++;
        }
    }
    
    return count;

}

//SORT THE FILES IN THE DIRECTORY and return a struct containing the sorted files and info on them
struct array_name *sort_fileNames(struct array_name *fileNames, int length,
                                    DIR *pDirectory, char* path)
{
    struct dirent *pDirent;
    struct stat sfile;
    int i = 0;
    char * filepath = malloc(1000*sizeof(char));
    struct passwd *userIDFileName;
    struct group *groupIDFileName;
    //read through the directory and concat the file name onto the end of the absolute path
    while((pDirent = readdir(pDirectory))!= NULL ) {
        filepath = strcpy(filepath,path);
        filepath = strcat(filepath,"/");
        filepath = strcat(filepath,pDirent->d_name);
        
        //Start System Call
        if(lstat(filepath, &sfile)!=0)
        {
            printf("The file/dir that was called on is %s\n",pDirent->d_name);
            perror("stat failed in the sort_fileNames");
        }
        

        if(strcmp(pDirent->d_name,".") == 0 || strcmp(pDirent->d_name,"..") == 0) continue;

        //Copied all the relevant file info to the fileName struct for each file
        strcpy(fileNames[i].name,pDirent->d_name);
        fileNames[i].s_mode = sfile.st_mode;
        fileNames[i].i_node = sfile.st_ino;
        userIDFileName = getpwuid(sfile.st_uid);
        groupIDFileName = getgrgid(sfile.st_gid);
        fileNames[i].groupID = malloc(10000*sizeof(char));
        fileNames[i].userID = malloc(10000*sizeof(char));
        fileNames[i].groupID = groupIDFileName->gr_name;
        fileNames[i].userID = userIDFileName->pw_name;
        fileNames[i].link = sfile.st_nlink;
        fileNames[i].size_file = sfile.st_size;

    


        fileNames[i].time = sfile.st_mtime;
        localtime_r(&fileNames[i].time,&timeStruct);
        i++;
        

        

    }

    //temporary string to store for sort
    char temp[10000];
    __mode_t tempMode;
    char * tempUser = malloc(1000*sizeof(char));
    char * tempGroup = malloc(1000*sizeof(char));
    long tempSize;
    __ino_t tempNode;
    long tempLink;
    time_t tempTime;

    for(int i = 0; i<length-1; i++) {
        for(int j = i+1; j<length; j++)  {

            //sort all the info together in the fileName struct 
            if((strcmp(fileNames[i].name,fileNames[j].name) > 0)) {
                strcpy(temp,fileNames[i].name);
                tempMode = fileNames[i].s_mode;
                tempUser = fileNames[i].userID;
                tempGroup = fileNames[i].groupID;
                tempSize = fileNames[i].size_file;
                tempNode = fileNames[i].i_node;
                tempLink = fileNames[i].link;
                tempTime = fileNames[i].time;


                strcpy(fileNames[i].name,fileNames[j].name);
                fileNames[i].s_mode = fileNames[j].s_mode;
                fileNames[i].userID = fileNames[j].userID;
                fileNames[i].groupID = fileNames[j].groupID;
                fileNames[i].size_file = fileNames[j].size_file;
                fileNames[i].i_node = fileNames[j].i_node;
                fileNames[i].link = fileNames[j].link;
                fileNames[i].time = fileNames[j].time;



                strcpy(fileNames[j].name, temp);
                fileNames[j].s_mode = tempMode;
                fileNames[j].userID = tempUser;
                fileNames[j].groupID = tempGroup;
                fileNames[j].size_file = tempSize;
                fileNames[j].i_node = tempNode;
                fileNames[j].link = tempLink;
                fileNames[j].time = tempTime;

            }
            
        }
    }

    free(filepath);
    return fileNames;
}


//this function executes the -R option recursively
void argumentR(struct array_name *sortedNames,int count,char * passedfilepath,
                int argHasi, int argHasl)
{
    
    int newcount;
    struct array_name *newsortedNames;
    struct lengths maxwidth;
    DIR *newDir;
    char* tempfilepath = malloc(1000*sizeof(char));
    int firstIterationPrint = 0;

    //find the max lengths of the files in the passed directory
    maxwidth = maxLengths(passedfilepath,count,sortedNames);

    //if we havent printed this directory before then print the -i and -l options for it
    if(firstIterationPrint == 0) {
        printf("\n'%s':\n",passedfilepath);
        for(int j =0; j < count;j++)
        {
                
                if(argHasi == 1)
                {
                    printf("%*ld ", maxwidth.ino+1,sortedNames[j].i_node);
                }
                if(argHasl == 1)
                {
                    printf((S_ISDIR(sortedNames[j].s_mode)) ? "d" : "-");
                    printf((sortedNames[j].s_mode & S_IRUSR) ? "r" : "-");
                    printf((sortedNames[j].s_mode & S_IWUSR) ? "w" : "-");
                    printf((sortedNames[j].s_mode & S_IXUSR) ? "x" : "-");
                    printf((sortedNames[j].s_mode & S_IRGRP) ? "r" : "-");
                    printf((sortedNames[j].s_mode & S_IWGRP) ? "w" : "-");
                    printf((sortedNames[j].s_mode & S_IXGRP) ? "x" : "-");
                    printf((sortedNames[j].s_mode & S_IROTH) ? "r" : "-");
                    printf((sortedNames[j].s_mode & S_IWOTH) ? "w" : "-");
                    printf((sortedNames[j].s_mode & S_IXOTH) ? "x" : "-");
                    printf(" ");

                    printf("%*ld ", maxwidth.link,sortedNames[j].link);
                    printf("%*s ", maxwidth.max_user,sortedNames[j].userID);
                    printf("%*s ", maxwidth.max_group,sortedNames[j].groupID);

                    printf("%*ld ", maxwidth.size,sortedNames[j].size_file);

                    localtime_r(&sortedNames[j].time, &timeStruct);
                    strftime(mtime, sizeof mtime, "%b %-2d %-y%y %H:%M", &timeStruct);
                    printf("%s ", mtime);
                                
                    
                    
                }
                printf("%-16s\n", sortedNames[j].name);

        }
        //record that we just printed this directory
        firstIterationPrint = 1;
    }

    //loop through every file in this directory
    for(int i = 0; i<count;i++)
    {   
        //check if the file is a directory and is not a symbolic link
        if(((S_ISDIR(sortedNames[i].s_mode))!=0) && (S_ISLNK(sortedNames[i].s_mode)==0))
        {
            //if it is a directory then we want to make a path inside of it
            //do this through concat
            tempfilepath = strcpy(tempfilepath,passedfilepath);
            tempfilepath = strcat(tempfilepath,"/");
            tempfilepath = strcat(tempfilepath, sortedNames[i].name);

            //open this found direcotry
            newDir = opendir(tempfilepath);
            //count the number of files in the directory
            newcount = countFiles(newDir);

            newsortedNames = (struct array_name*) malloc(newcount*sizeof(struct array_name));
            closedir(newDir);

            //reopn the dir
            newDir = opendir(tempfilepath);
            //sort the files in the directory
            newsortedNames = sort_fileNames(newsortedNames,newcount,newDir,tempfilepath);
            //recall argumentR() using the infomation on this newDir we found
            argumentR(newsortedNames,newcount,tempfilepath, argHasi,argHasl);
            //reset the file path to the one that access the directory this instance of 
            //argumentR() is in
            tempfilepath = strcpy(tempfilepath,passedfilepath);

            free(newsortedNames);
            
        }
    }
    //if you reached here then there are no more directories to enter
   
    free(tempfilepath);
    return;
}


