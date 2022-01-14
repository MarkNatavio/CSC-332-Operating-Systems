// Lab 4
// Mark Natavio
// PART 1 Simple Command Interpreter
// Write a special simple command interpreter that takes a command and its arguments. This interpreter is a program where the 
// main process creates a child process to execute the command using exec() family functions. After executing the command, it 
// asks for a new command input ( parent waits for child). The interpreter program will get terminated when the user enters quit.

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main (int argc, char* argv[]) {
	// Prepare values
	int command;
	int cpid;

	char user_input[64];
	char *token;
	char *arg[20] = {NULL};
	
	// Ask for command
	printf("Enter command: ");
	scanf("%[^\n]s", user_input); // Scan input

	// Terminate if user enters quit.
	while ((strcmp(user_input, "quit"))) { 
		token = strtok(user_input, " "); // Get user input
    
		int index = 0;
		while(token != NULL) {
			arg[index] = token; 
	    token = strtok(NULL," ");  
			index++;
		}

		cpid = fork(); // create child process
    wait(&command);
		if (cpid == 0) {
			execvp (arg[0],arg); // execvp command
	  	printf("Wrong command, try again.\nEnter command: "); // if command is not acceptable, warn user
			scanf(" %[^\n]s", user_input);
		} else {
			for (int i = 0; i < index; ++i){
				arg[i] = NULL; 
			}
			// Ask for commmand
			printf("\nEnter command: ");
			scanf(" %[^\n]s", user_input); // Get user input
		}		
	}
	return 0;
}