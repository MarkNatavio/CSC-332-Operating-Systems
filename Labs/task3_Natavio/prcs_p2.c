// Lab 3
// Mark Natavio
// [Step 2] Prcs_P2.c: Copy the contents of source.txt into destination1.txt and destination2.txt asper the following procedure.
//                     1. Read the next 100 characters from source.txt, and among characters read, replace each char-acter ’1’
//                        with character ’L’ and all characters are then written in destination1.txt

//                     2. Then the next 50 characters are read from source.txt, and among characters read, replace each character
//                        ’3’ with character ’E’ and all characters are then written in destination2.txt.

//                     3. The previous steps are repeated until the end of file source.txt. The last read may not have 100 or 50
//                        characters.

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main (int argc, char* argv[]) {
  int source = open("source.txt", O_RDONLY); // Open file in read
  int destination1 = open("destination1.txt", O_WRONLY); // Open file in write mode or create if it doesn't exist
  int destination2 = open("destination2.txt", O_WRONLY);

  // Check if any of the files failed to open
  if(source < 0){
    printf("\nFailed to open source.txt file");
    perror("open");
    return 1;
  
  } else if (destination1 < 0) {
    printf("\nFailed to open destination1.txt file");
    perror("open");
    return 1;
  
  } else if (destination2 < 0){
    printf("\nFailed to open destination2.txt file");
    perror("open");
    return 1;
  
  } else { // All files work, now we copy content into destination1.txt and destination2.txt
    int read_file;
    char buffer[150];
    char buffer2[150];
    
    while((read_file = read(source,buffer,150)) != 0) {
      // Error message if failed to read
      if (read_file < 0) {
        printf("Fail to read.");
        perror("read \n");
        return 1;
      }

      // 1. Read the next 100 characters from source.txt, and among characters read, replace each char-acter ’1’ with character ’L’ and all characters are then written in destination1.txt
      for(int i = 0; i < 100; i++) {
        if(buffer[i] == '1') {
          buffer[i] = 'L';
        }
      }
      for(int i = 100; i < 150; i++) {
        if(buffer[i] == '1') {
          buffer[i] = '1';
        }
      }
      read_file = write(destination1, buffer, read_file); // Write to destination1.txt
      if (read_file < 0) { // Error message if failed to write
        printf("Fail to write to destination1.txt");
        perror("write \n");
        return 1;
      }


      // 2. Then the next 50 characters are read from source.txt, and among characters read, replace each character ’3’ with character ’E’ and all characters are then written in destination2.txt.
      for(int i = 0; i < 100; i++) {
        buffer2[i] = buffer[i];
        if (buffer2[i] == 'L'){
          buffer2[i] = '1';
        }
      }
      for(int i = 100; i < 150; i++) {
        buffer2[i] = buffer[i];
        if (buffer2[i] == '3') {
          buffer2[i] = 'E';
        }
      }
      read_file = write(destination2, buffer2, read_file); // Write to destination1.txt
      if (read_file < 0) { // Error message if failed to write
        printf("Fail to write to destination2.txt");
        perror("write \n");
        return 1;
      }

    }
  }

  // Closing files
  if (close(source) < 0) {
    printf("Fail to close source file");
    perror("close");
    return 1;
  
  } else if (close(destination1) < 0) {
    printf("Fail to close destination1.txt");
    perror("close");
    return 1;
  
  } else if (close(destination2) < 0) {
    printf("Fail to close destination2.txt");
    perror("close");
    return 1;
  
  } else {
    printf("\nClosed Succesfully. \n");
  }

  return 0;
}

