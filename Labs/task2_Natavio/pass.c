// Lab 2
// Mark Natavio, EMPLID: 23759236
// Task 4. Open a file called readme.txt in the child process, read the contents and pass to the parent process.
//         Parent process will write to readme.txt, “Parent is writing:” and write the contents it received from the
//         child to the readme.txt file. (Hint: Use Pipe)

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int pipefd[2];
  pid_t cipid;
  char buf[100] = "";

  if (pipe(pipefd) == -1) { // create pipe
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  cipid = fork();
  if (cipid == -1){
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if (cipid == 0) { // Child Process, will read from file
    int read_file;
    int fd = open("readme.txt", O_RDONLY); // read the readme.txt file

    while((read_file = read(fd,buf,1)) != 0){ // reading the file contents
      if (read_file < 0){
        printf("Fail to read.");
        perror("read \n");
        return 1;
      }
      write(pipefd[1], buf, 1); // write contents of file into pipefd[1]
    }
    
    close(pipefd[0]);
    exit(EXIT_SUCCESS);
  } else { // Parent process, will write into file
    close(pipefd[1]);
    int read_file;
    char cont[100] = "";
    int fd = open("readme.txt", O_WRONLY); // open readme.txt file as a write only

    while((read_file = read(pipefd[0],buf,1)) != 0){ // read the pipefd[0]
      if (read_file < 0){
        printf("Fail to read.");
        perror("read \n");
        return 1;
      }
      write(fd, buf, 1); // keep contents of file
      strcat(cont, buf); // save contents of file into a char
    }
    write(fd, "Parent is writing: ", strlen("Parent is writing: ")); // write "Parent is writing: " into file
    write(fd, cont, strlen(cont)); // write contents into file 
    
    close(pipefd[0]);
    wait(NULL); // wait for child
    exit(EXIT_SUCCESS);
  }
return 0;
}