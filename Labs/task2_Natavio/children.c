// Lab 2
// Mark Natavio, EMPLID: 23759236
// Task 1. Write a program children.c, and let the parent process produce two child processes. One prints
//         out "I am child one, my pid is: " PID, and the other prints out "I am child two, my pid is: " PID.
//         Guarantee that the parent terminates after the children terminate (Note, you need to wait for two
//         child processes here). Use the getpid() function to retrieve the PID

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
  // Setting up
  int wait_val;
  int wait_status;

  // Child 1 process
  int child1 = fork();

  if (child1 == 0){
    //code specifying how the child process behaves
    printf("In the child 1 process");
    printf("\nI am child one, my pid is: %d", getpid());
    return 0;
  }
  else if (child1 > 0){
    wait_val = waitpid(child1, &wait_status, 0);
		if (wait_val < 0) {
			  perror("wait");
			  return 1;
		}
  }
  else if (child1 < 0){
    printf("Error occured while creating the process for child 1");
    return 0;
  }

  // Child 2 process
  int child2 = fork();

  if (child2 == 0){
    //code specifying how the child process behaves
    printf("\n\nIn the child 2 process");
    printf("\nI am child two, my pid is: %d", getpid());
    return 0;
  }
  else if (child2 > 0){
    wait_val = waitpid(child2, &wait_status, 0);
		if (wait_val < 0) {
			  perror("wait");
			  return 1;
		}
  }
  else if (child2 < 0){
    printf("\nError occured while creating the process for child 2");
    return 0;
  }
  return 0;
}