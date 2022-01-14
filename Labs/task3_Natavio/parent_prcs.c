// Lab 3
// Mark Natavio
// [Step 3] Using fork create a child process, say Child 1 that executes Prcs_P1. This will create two destinationfiles according to Step 1.
// [Step 4] After Child 1 finishes its execution, use fork to create another child process, say Child 2 and execute Prcs_P2that accomplishes 
//          the procedure described in Step 2.

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char* argv[]) {
	// Setting up variables
	int cpid1 = 0;
  int cpid2 = 0;
  int wait_status = 0;
  int wait = 0;

	// Make child process 1
	cpid1 = fork();
	if (cpid1 < 0) { // Check for error with fork
		printf("Failed to fork child1. \n");
		perror("fork");
		return 1;
	}
	// Execute prcs_p1 using execv with child1
	if (cpid1 == 0) {
		char *args[] = {NULL};
		execv("./prcs_p1.out", args);
	} else {
		wait = waitpid(cpid1, &wait_status, 0);
		if (wait < 0) {
			printf("Error with wait. \n");
			perror("wait");
			return 1;
		}
	}

	// Make child process 2
	cpid2 = fork();
	if (cpid2 < 0) { // Check for error with fork
		printf("Failed to fork child2. \n");
		perror("fork");
		return 1;
	}
	// Execute prcs_p2 using execv with child2
	if (cpid2 == 0) {
		char *args[] = {NULL};
		execv("./prcs_p2.out", args);
	} else {
		wait = waitpid(cpid2, &wait_status, 0);
		if (wait < 0) {
			printf("Error with wait. \n");
			perror("wait");
			return 1;
		}
	}

	return 0;
}