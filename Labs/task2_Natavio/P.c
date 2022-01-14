// Lab 2
// Mark Natavio, EMPLID: 23759236
// Task 2. Consider the parent process as P. The program consists of fork() system call statements placed at
//         different points in the code to create new processes Q and R. The program also shows three variables:
//         a, b, and pid - with the print out of these variables occurring from various processes. Show the values
//         of pid, a, and b printed by the processes P, Q, and R.

#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]){
  //parent P
  int a = 10, b = 25, fq = 0, fr = 0;

  // Process Q
  fq = fork(); // fork a child - call it Process Q
  if(fq == 0){ // Child successfully forked
    printf("\nValue 0");
    a = a+b;
    printf("\na: %d", a);
    printf("\nb: %d", b);
    printf("\nprocess_id: %d\n", getpid());

    // Process R
    fr = fork(); // fork another child - call it Process R
    if(fr != 0){
      printf("\nValue 1");
      b = b+20;
      printf("\na: %d", a);
      printf("\nb: %d", b);
      printf("\nprocess_id: %d\n", getpid());
    }
    else{
      printf("\nValue 2");
      a=(a*b)+30;
      printf("\na: %d", a);
      printf("\nb: %d", b);
      printf("\nprocess_id: %d\n", getpid());
    }
  }
  else{
    printf("\nValue 3");
    b = a+b -5;
    printf("\na: %d", a);
    printf("\nb: %d", b);
    printf("\nprocess_id: %d\n", getpid());
  }

  return 0;
}