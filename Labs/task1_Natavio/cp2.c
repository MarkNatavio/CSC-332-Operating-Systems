// Lab 1
// Task 4: Repeat part 2 (by writing a new C program) as per the following procedure:
//         (a) Read the next 100 characters from source.txt, and among characters read, replace
//             each character `1` with character `L` and all characters are then written in
//             destination.txt
//         (b) Write characters "XYZ" into file destination.txt
//         (c) Repeat the previous steps until the end of file source.txt. The last read step may not
//             have 100 characters. 
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
    int read_file;
    int count = 100;
    char buffer[count];
    
    while((read_file = read(SourceFile,buffer,count)) != 0)
    {
      // Error message if failed to read
      if (read_file < 0)
      {
        printf("Fail to read.");
        perror("read \n");
        return 1;
      }

      // Otherwise, replace every '1' character with  'L' character
      for(int i = 0; i < count; i++)
      {
        if(buffer[i] == '1')
        {
          buffer[i] = 'L';
        }
      }

      // Write to destination.txt
      read_file = write(DestinationFile, buffer, read_file);
      read_file = write(DestinationFile, "XYZ", 3);

      // Error message if failed to write
      if (read_file < 0)
      {
        printf("Fail to write.");
        perror("write \n");
        return 1;
      }
    }
    // Print to console copying and replacing has been done
    printf("Write Succesfully.");
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