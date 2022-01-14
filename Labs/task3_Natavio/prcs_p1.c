// Lab 3
// Mark Natavio
// Part 3 [Step 1] Prcs_P1.c: Create two files namely, destination1.txt and destination2.txt with read, write,
//                 and execute permissions.

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  
  // Creating file destination1
  int destination1 = open("destination1.txt", O_RDONLY | O_WRONLY | O_CREAT, 00700); // open file and read only or create
  if (destination1 < 0) { // Check if file can be opened
    printf("Sorry, I couldn't open destination1.txt\n");
    perror("open"); /* This will explain why */
    return 1;

  } else {
    printf("\nI successfully created destination1.txt\n"); // say we opened successfully
  }
  if (close(destination1) < 0) {
    printf("Fail to close destination1.txt");
    perror("close");
    return 1;
  
  }

  // Creating file destination2
  int destination2 = open("destination2.txt", O_RDONLY | O_WRONLY | O_CREAT, 00700); // open file and read only or create
  if (destination2 < 0) { // Check if file can be opened
    printf("Sorry, I couldn't open destination2.txt\n");
    perror("open"); /* This will explain why */
    return 1;

  } else {
    printf("\nI successfully created destination2.txt\n"); // say we opened successfully
  }
  if (close(destination2) < 0) {
    printf("Fail to close destination2.txt");
    perror("close");
    return 1;
  
  }

  printf("\nClosed both files succesfully. \n");
  return 0;
} 