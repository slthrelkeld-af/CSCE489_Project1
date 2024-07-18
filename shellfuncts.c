/*************************************************************************************
 * shellfuncts.c - code definitions for your functions
 * Threlkeld
 * References listed in myshell.c
 *************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shellfuncts.h"

void dir(){
	/* List all files in current directory */
	execl("/bin/ls","ls",NULL);
	exit(0);//doesn't actually do anything since ls exits on its own, but just to be sure
	return;
}

void create (char fileName[], char silent) {
	//length 255 set based on googling max unix filename length
	FILE* myFile;
	myFile = fopen (fileName, "r"); //check for file existence
	if (myFile == NULL)
	{
		myFile = fopen(fileName, "w");
		fclose(myFile);
		if (silent != '&')
		{
			printf ("file %s created \n", fileName);
		}
	}
	else
	{
		if (silent != '&')//will not notify of existing file if backgrounded
		{
			printf ("file %s already exists \n", fileName);
		}
	}
	exit(0);
}

void update (char fileName[], int numberRepeats, char myText[], char silent){

	FILE* myFile;
	
	//file manipulation begins
	myFile = fopen(fileName, "a+");

	for (int i = 0; i < numberRepeats; i++)
	{
		fprintf(myFile, "%s\n" ,myText);
		fflush(myFile);
		sleep((unsigned int)strlen(myText)/5);
	}
	fclose (myFile);
	//file manipulation ends
	
	if (silent != '&'){
		printf("process %i has updated %s\n", getpid(), fileName);
	}
}

void list (char fileName[]){
	FILE* myFile;
	myFile = fopen (fileName, "r");
	if (myFile == NULL)
	{
		printf("File not found\n");
		exit(0);
	}
		
	execl("/usr/bin/cat", "cat", fileName, NULL);
	exit(0);
}


