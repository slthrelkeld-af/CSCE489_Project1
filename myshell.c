/*************************************************************************************
 * myshell - student code for Project 1 of CSCE 489 
 * Threlkeld
 *
 *************************************************************************************/
//https://man7.org/linux/man-pages/index.html used for reference on system calls & libraries to import
//w3schools.com & geeksforgeeks.org used for general syntax & command reference, including clearing input buffer
//https://www.educative.io/answers/splitting-a-string-using-strtok-in-c referenced for splitting a string
//ibm.com/docs for documentation on waitpid

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shellfuncts.h"

int main(int argv, const char *argc[]) {
(void) argv; // Make compile warnings go away - be sure to delete this line if you use the param
(void) argc; // Make compile warnings go away - be sure to delete this line if you use the param

//int commandCount = 0;
char argumentArray[5][255]; //will be used to store command and arguments, length set based on googling max unix filename length

do
{
	//commandCount ++;//can be used to impliment prevention of runaway/infinite loop command entries

	char argumentArray[5][255] = {"\0","\0","\0","\0","\0"}; //clear command array
	char commandLine[255] = "\0";
	int linePointer = 0;
	int argPointer = 0;
	int argCharPointer = 0;
	int childReturnStatus;

	printf("shell PID: %i, Command>", getpid());
	scanf("%[^\n]", commandLine);
	//printf("input is %s\n", commandLine); //testing line acceptance

	int commandLen = strlen(commandLine);
	int quoteLock = 0;
	
	//input parser loop
	while (commandLine[linePointer] != '\0' && linePointer < commandLen)//added commandLen as error prevention
	{
	
		if (commandLine[linePointer] == ' ' && quoteLock == 0) //default behavior on space. not sure if it's more efficient for this to be first check or not, but it works
		{
			while (commandLine[linePointer] == ' ' && quoteLock == 0)//skip whitespace (prevents multiple spaces displacing args)
			{
				linePointer++;
			}
			argumentArray[argPointer][argCharPointer] = '\0';//appropriately terminate string
			argPointer++;//move to next arg
			argCharPointer = 0;//reset pointer position
		}
		else if (commandLine[linePointer] == '\"')
		{
			if (quoteLock == 0) {
				quoteLock = 1; //turn on lock
				linePointer++; //skip triggering quote. arg string swap taken care of by space before the quote
			}
			else { //if quotelock is on and you hit a quote, close that arg string and move to next arg
				argumentArray[argPointer][argCharPointer] = '\0';
				argPointer++;
				argCharPointer = 0;
				quoteLock = 0;
				linePointer ++;//skip quote
				while (commandLine[linePointer] == ' ' && quoteLock == 0)//skip trailing whitespace (prevents multiple spaces displacing args)
				{
					linePointer++;
				}
			}
			
		}
		else
		{
			argumentArray[argPointer][argCharPointer] = commandLine[linePointer];
			argCharPointer++;
			linePointer++;
		}
		

	}

	//clear input buffer to prevent infinite loop
	while (getchar() != '\n');

	/*//output arg array contents to test array storage is working right
	for (int j = 0; j < 5; j++)
	{
		if (argumentArray[j][0] != '\0'){
		printf("command array line %d is %s\n", j, argumentArray[j]);
		}
	}
	*/


	if (strcmp (argumentArray[0], "halt") == 0) //halt, exits program
	{
		exit(0);
	}

	else
	{
		__pid_t forkProcess;
		forkProcess = fork();

		if (forkProcess < 0)
		{
			printf("fork error");
		}

		else if (forkProcess > 0)
		{
			printf("child PID: %i\n", (int)forkProcess);
			if (argumentArray[2][0] != '&' && argumentArray[4][0] != '&')
			{
				//forkProcess = wait(NULL); //had to change waits, as execl was behaving poorly with wait(NULL);
				forkProcess = waitpid(forkProcess, &childReturnStatus, 0);
			}
			// need to allow for paralell processing with & (eg remove the wait)
		}

		else if (forkProcess == 0) //child process
		{
			if (strcmp(argumentArray[0], "dir") == 0) //dir, lists directory objects.
			{
				if (argumentArray[1][0] != '&'){ //do not execute display command if backgrounded
					dir();
				}
			}
			
			else if (strcmp(argumentArray[0], "create") == 0)//create <name>, creates a file named by the arg
			{
				create(argumentArray[1], argumentArray[2][0]);
			}
			
			else if (strcmp(argumentArray[0], "list") == 0)//list <name>, cats the contents of a file named by the arg
			{
				if (argumentArray[2][0] != '&'){ //do not execute display command if backgrounded
					list(argumentArray[1]);
				}
				//need to check for ongoing writes
			}
			
			else if (strcmp(argumentArray[0], "update") == 0)//update <name> <number> <text>, fills a file with a given number of repeated pieces of text
			{
				if (atoi(argumentArray[2]) < 1) //error check - atoi will return 0 for non-integers, however it CAN extract partial numbers, aka 23D will become 23
					{
						printf("invalid number or non-number of repeats requested\n");
						exit(0);
					}
					//testing has not shown issues with simultaneous writes other than broken sequences of repeats.
					//Write lock not implemented in this version of this shell.
								
				update(argumentArray[1],atoi(argumentArray[2]),argumentArray[3],argumentArray[4][0]);
			}

			else
			{
				printf("Command not recognized\n");
			}

		exit(0); //exit for all child processes, in cases where it is not called elsewhere
		}
		
	}

} while (strcmp(argumentArray[0], "halt") != 0); //might as well check for "halt" again since we need to maintain the loop anyway
exit (0);
}