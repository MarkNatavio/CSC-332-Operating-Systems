// Lab 3
// Mark Natavio
// Part 1 Write a program where a child is created to execute command that tells you the date and time in Unix.
//        Use execl(...).
//        Note, you need to specify the full path of the file name that gives you date and time information.
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
		execl("/bin/date", "date", NULL); // print date and time using execl

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