Hello reader,

This repository displays a manually programmed version of the "ls" function from the UNIX file system. 
The ls function in the the UNIX file system is a function that allows the user to display file information using the command line. The version
of ls that I have programmed here is called "myls". This version of the function implements the default functionality of ls, as well as different 
command line arguments that can be utilized to display more detailed information.

## Files

### myls.c:
	This is the file which hosts the main function. This file is responsible for parsing through the command line arguments and controlling the work flow.
	
mylsfunctions.c:
	This file hosts the functions which are called in main. The important functions in this file are those that implement each command line argument 
	(All possible arguments are referenced below).
	
mylsfunctions.h:
	This is the function header file for mylsfunctions.c.
	
makefile:
	The command "make" compiles myls.c with all necessary dependencies.

## Possible Command Line Arguments

### ./myls (no arguments):

Running the code with no command line arguments will result in simply displaying the files and directories within the current directory. This is nearly identical to running the native "ls" command.

./myls -i 
	This will present all files along with their i-nodes.
	
./myls -l 
	This will present all files along with a list of info including
	date created, file permissions, userID, and groupID.
	
./myls -R
	This will run a recursive search through the current file directory as well as any 
	other file directories it finds. It will then display all info specified by the other
	arguments included.


This project was created on a time constraint and can definitely be improved upon as there is some bad code convention, but overall I appreciate the tools that were learned from participating in this assignment.
