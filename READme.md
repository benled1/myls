Hello reader,

This repository displays a manually programmed version of the "ls" function from the UNIX file system. 
The ls function in the the UNIX file system is a function that allows the user to display file information using the command line. The version
of ls that I have programmed here is called "myls". This version of the function implements the default functionality of ls, as well as different 
command line arguments that can be utilized to display more detailed information.

All output examples are run on a test directory with the following structure:

![image](https://user-images.githubusercontent.com/55040326/127197820-f8083528-1798-4206-9c20-5d8ffad23370.png)

## Files

### myls.c:

This is the file which hosts the main function. This file is responsible for parsing through the command line arguments and controlling the work flow.
	
### mylsfunctions.c:

This file hosts the functions which are called in main. The important functions in this file are those that implement each command line argument 
(All possible arguments are referenced below).

### mylsfunctions.h:

This is the function header file for mylsfunctions.c.
	
### makefile:

The command "make" compiles myls.c with all necessary dependencies.

## Possible Command Line Arguments
There are two types of command line arguments that can be passed to the myls function. The first type is the argument indicates the root directory that needs to be printed. 
The second type of argument indicates in which way the information about that directory will be printed.

This second type of argument is a combination of the three letters i,l, and R. Each of these letters indicate something about how the file will be displayed. They will be explained further below.

### ./myls (no arguments) Test:

Running the code with no command line arguments will result in simply displaying the files and directories within the current directory. This is nearly identical to running the native "ls" command. 

Running this command on the test directory produces the following results:

![image](https://user-images.githubusercontent.com/55040326/127198124-4d89d4b7-d9ba-434c-828c-e94f8e54b89e.png)




### ./myls -i Test:

This will present all files the same as the no argument case except it will add the i-node to be displayed alongside the filename.

Running this command on the test directory produces the following results:

![image](https://user-images.githubusercontent.com/55040326/127201650-7fa82401-0796-48df-87cf-a3ba240fc95f.png)


	
### ./myls -l Test:
This will present all files the same as the no argument case, along with a list of info including date created, file permissions, userID, and groupID.

Running this command on the test directory produces the following results:

![image](https://user-images.githubusercontent.com/55040326/127201834-bc4687c1-1153-4941-b08a-74e942214c60.png)



	
### ./myls -R Test:

This will run a recursive search through the current file directory as well as any other file directories it finds. It will then display all info specified by the other
arguments.

Running this command on the test directory produces the following results:

![image](https://user-images.githubusercontent.com/55040326/127202189-ad8b1965-65c2-4fd6-bb8d-ee0d49916ec3.png)

### ./myls -ilR Test:

As mentioned above, the command can be run with any combination of the previous three arguments.

Running this command on the test directory produces the following results:

![image](https://user-images.githubusercontent.com/55040326/127202397-36d81c45-009b-4587-8acc-cb70076f5d7e.png)


Thanks for reading through my project. Feel free to check out the source code if you're interested.

