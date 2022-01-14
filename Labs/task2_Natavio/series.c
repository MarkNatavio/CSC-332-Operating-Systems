// Lab 2
// Mark Natavio, EMPLID: 23759236
// Task 3. Consider a series, S1 = 2 +4 +6+,..+10 and another series S2=1+3+5+..+9. and another series S3 = 1+2+3+…+10.
//         We know that S1+S2 = S3.
//         Now write a program,where a parent process creates 2 child process and computes S1 and S2. And Parent
//         process computes S3. The input argument for program will be the end of series number for S3.

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  // Setting up
  int limit = atoi(argv[1]);
  int S1 = 0;
  int S2 = 0;
  int S3 = 0;
  int wait_val;
  int wait_status;

  // Child 1 process
  int child1 = fork();
  if (child1 == 0){
    printf("In the child 1 process");
    printf("\nI am child one, my pid is: %d", getpid());
    printf("\nParent is: %d", getppid());
    printf("\nComputing S1:\n");
    for (int i = 1; i <= limit; i++){
      if (i%2 != 0){
        S1 += i;
        if(i+1 >= limit){
          printf("%d = %d", i, S1);
        } else {
          printf("%d + ",i);
        }
      }
    }
    printf("\nS1 = %d", S1);
    return 0;
  }
  else if (child1 > 0){
    wait_val = waitpid(child1, &wait_status, 0);
		if (wait_val < 0) {
			  perror("\nwait");
			  return 1;
		}
  }
  else if (child1 < 0){
    printf("\nError occured while creating the process for child 1");
    return 0;
  }

  // Child 2 process
  int child2 = fork();
  if (child2 == 0){
    //code specifying how the child process behaves
    printf("\n\nIn the child 2 process");
    printf("\nI am child two, my pid is: %d", getpid());
    printf("\nParent is: %d", getppid());
    printf("\nComputing S2:\n");
    for (int i = 2; i <= limit; i++){
      if (i%2 != 1){
        S2 += i;
        if(i+1 >= limit){
          printf("%d = %d", i, S2);
        } else {
          printf("%d + ",i);
        }
      }
    }
    printf("\nS2 = %d", S2);
    return 0;
  }
  else if (child2 > 0){
    wait_val = waitpid(child2, &wait_status, 0);
		if (wait_val < 0) {
			  perror("\nwait");
			  return 1;
		}
  }
  else if (child2 < 0){
    printf("\nError occured while creating the process for child 2");
    return 0;
  }

  printf("\n\nIn the parent process");
  printf("\nMy pid is: %d", getpid());
  printf("\nComputing S3:\n");
  for (int i = 1; i <= limit; i++){
    S3 += i;
    if(i+1 > limit){
      printf("%d = %d", i, S3);
    } else {
      printf("%d + ",i);
    }
  }
  printf("\nS3 = %d", S3);

  return 0;
}