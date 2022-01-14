// Lab 1
// Task 2: Write a C program to implement a command called displaycontent that takes a (text) file
//         name as argument and display its contents. Report an appropriate message if the file does
//         not exist or can’t be opened (i.e. the file doesn’t have read permission). You are to use open(),
//         read(), write() and close() system calls. 
// Mark Natavio

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main (int argc, char* argv[]) 
{
	int fd;
  errno = 0;
  char* filepath = argv[1];
  fd = open(argv[1], O_RDONLY); // Open file, read only
  if(fd < 0)
  {
    printf("Sorry, I couldn't open %s\n", filepath);
    perror("open"); /* This will explain why */
    return 0;
  }
  else
  {
    // check if we can access file content
    int file_content = lseek(fd,0,SEEK_END);
    if (file_content < 0)
    {
      perror("lseek"); /* This will explain lseek error */
      return 1;
    }

    char buffer[file_content]; // Get file content
    lseek(fd,0,SEEK_SET);
    
    // Read file
    int read_file = read(fd, buffer, sizeof(buffer));
    if (read_file < 0) // check if there's an error with read system call
    {
      printf("Fail to read.");
      perror("read\n");
      return 1;
    }
    printf ("\nCan read file %s\n", filepath);
    
    // Write and display file content
    int write_file = write(1, buffer, read_file);
    if (write_file < 0) // check if there's an error with write system call 
    {
      printf("Fail to write.");
      perror("write \n");
      return 1;
    }
  }

  // Closing file
  if (close(fd) < 0)
  {
    printf("Sorry, I couldn't close %s\n", filepath);
    perror("close");
    return 1;
  } 
  else
  {
    printf("\nI succesfully closed %s\n", filepath);
  }
  return 0;
}