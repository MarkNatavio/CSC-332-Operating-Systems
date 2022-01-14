// Lab 1
// Task 1 (b): Write a C program where open system call creates a new file (say, destination.txt) and
//             then opens it. (Hint: use the bitwise OR flag) 
// Mark Natavio

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int fd = open("destination.txt",O_RDONLY|O_CREAT); // open file and read only or create
  
  // Check if file can be opened
  if(fd < 0)
  {
    printf("Sorry, I couldn't open destination.txt\n");
    perror("open"); /* This will explain why */
    return 1;
  }
  else
  {
    printf("\nI successfully opened destination.txt\n"); // say we opened successfully
    
    // Closing file
    if (close(fd) < 0)
    {
      printf("Sorry, I couldn't close destination.txt\n");
      perror("close");
      return 1;
    } 
    else
    {
      printf("\nI succesfully closed destination.txt\n");
    }
  }
  return 0;
} 