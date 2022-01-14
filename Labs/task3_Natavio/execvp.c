// Lab 3
// Mark Natavio
// Part 2 Write a program where a child is created to execute a command that shows all files (including hid-den files) 
//        in a directory with information such as permissions, owner, size, and when last modified.
//        Use execvp(...).
//        For the command to list directory contents with various options, refer the handout on Unix filesystem sent to
//        you in the first class.
//        Announce the successful forking of child process by displaying its PID.

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	// Setting up variables
	int cpid = 0;
	int wait_val = 0;
	int wait_status = 0;
	
	// Make child
	cpid = fork();

	if (cpid == 0) { // Child process
		printf("The fork was successful!\n"); // say fork was successful
		printf("I am the child process, my PID is: %d\n", getpid()); // print child process pid

		char *args[] = {"-a","-l", NULL};
		execvp("ls", args); // show all files in directory using execvp

	} else if (cpid > 0) { // Parent process
		wait_val = waitpid(cpid, &wait_status, 0);
		if (wait < 0) {
			perror("wait");
			return 1;
		}

	} else { // Check for fork error
		printf("Error occured while creating the child process.\n");
		return 0;
	}
	
	return 0;
}