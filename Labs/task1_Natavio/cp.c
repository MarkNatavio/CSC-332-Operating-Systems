// Lab 1
// Task 3: The cp command copies the source file specified by the SourceFile parameter to the
//         destination file specified by the DestinationFile parameter. 
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
	int SourceFile;
  int DestinationFile;
  errno = 0;
  SourceFile = open(argv[1], O_RDONLY); // Open file in read
  DestinationFile = open("destination.txt", O_WRONLY); // Open file in write mode or create if it doesn't exist

  // Check if any of the files failed to open
  if(SourceFile < 0)
  {
    printf("\nFailed to open source file");
    perror("open");
    return 1;
  } 
  else if (DestinationFile < 0)
  {
    printf("\nFailed to open destination file");
    perror("open");
    return 1;
  }
  else
  {
    // Print content of source.txt to destinatioon.txt
    int file_content = lseek(SourceFile,0,SEEK_END);
    if (file_content < 0)
    {
      perror("lseek"); /* This will explain lseek error */
      return 1;
    }
    char buffer[file_content];
    lseek(SourceFile,0,SEEK_SET);

    // Read the SourceFile
    int read_source_file = read(SourceFile, buffer, sizeof(buffer));
    if (read_source_file < 0) // Error with read system call of the sourcefile
    {
      printf("Fail to read source file");
      perror("read \n");
      return 1;
    }

    // Write to destination.txt and error with write system call
    int write_destination_file = write(DestinationFile, buffer, read_source_file);
    if (write_destination_file < 0)
    {
      printf("\nFailed to write into destination.txt");
      perror("write");
      return 1;
    } 
    else
    {
      printf("\nCopy Succesfully.");
    }
  }

  // Close file and error with close system call
  if (close(SourceFile) < 0)
  {
    printf("Fail to close source file");
    perror("close");
    return 1;
  }
  else if (close(DestinationFile) < 0)
  {
    printf("Fail to close destination.txt");
    perror("close");
    return 1;
  }
  else
  {
    printf("\nClosed Succesfully. \n");
  }

  return 0;
}