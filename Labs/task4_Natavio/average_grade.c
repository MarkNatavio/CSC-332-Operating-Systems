// Lab 4
// Mark Natavio
// Part 2
// There are 10 students enrolled in a course. The course covers x number of
// chapters from a textbook (x > 1).

// In each chapter y number of homeworks are assigned (y>=1). The average grade
// for each homework in all the chapters need to be found out.

// To solve this, write program which has the main process as Director process, 
// which reads a file containing grades of all homeworks of all chapters and creates 
// x number of Manager processes. Each Manager process will take care of solving a 
// chapter. Each manager process will create y number of Worker process and pass 
// marks of 10 students to each of them and they calculate and print the average.

// The input file should contain the data according to the value of x and y. 

// Processes Levels:
//
//                  Director
//            _____/        \______
//           /                     \
//       Manager                  Manager
//      /       \                /      \
// Worker 1    Worker 2     Worker 3   Worker 4

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char* argv[]) { 
  // Prepare values
  int x;
  int y;
  const int num_of_students = 10;
  int student_counter = 0;
  int ch_hw_counter = 0;
  int process_lv;
  double total_score = 0; 

  // Getting user input
  printf("\nEnter value of x (# of chapters): "); 
  scanf("%d", &x);
  printf("Enter value of y (# of homeworks): "); 
  scanf("%d", &y);

  printf("\nAverages:\n");
  const int total_hws = x*y;
  double quiz_grades [total_hws][num_of_students];
  double avg [total_hws];

  FILE * file = fopen("quiz_grades.txt", "read"); // Using the source file to open

  //while(student_counter < num_of_students) {
  for (student_counter = 0; student_counter < num_of_students; student_counter++) {
    for (ch_hw_counter = 0; ch_hw_counter < total_hws; ch_hw_counter++) {
      fscanf(file, "%lf", &quiz_grades[ch_hw_counter][student_counter]); // get values from file
    }
  }
  // reset counters
  student_counter = 0;
  ch_hw_counter = 0;

  for (int ch_counter = 0; ch_counter < x; ch_counter++) {
    wait(&process_lv);

    //Manager processes.
    pid_t manager_process = fork(); // Making x number of manager processes
    if(manager_process == 0) {
      ch_hw_counter = y * ch_counter; // get total # of hws for all chapters
      for(int hw_counter = 0; hw_counter < y; hw_counter++) {

        //Worker process
        wait(&process_lv);
        pid_t worker_process = fork(); // Making y number of worker processes
        if(worker_process == 0) {
          for (student_counter = 0; student_counter < num_of_students; student_counter++){
            total_score += quiz_grades[ch_hw_counter][student_counter]; // total sum of scores for specific HW
          }
          avg[ch_hw_counter] = total_score / num_of_students; // get average (total score sums / # of students)
          printf("Chapter %d, homework %d avg: %f\n",(ch_counter + 1),(hw_counter + 1), avg[ch_hw_counter]); // output
          exit(0); 
        } else if (worker_process < 0) { // Worker process failed
          perror("Worker Error");
        } else {
          if(wait(&process_lv) == -1) { // At wrong level
            printf("Worker Error\n");
          }
        }
        ch_hw_counter++; // go to next chapter hws
      }
      exit(0);
    }
    else if(manager_process < 0) { // Manager process failed
      perror("Manager Error");
      return 1;
    }
    else {
      if(wait(&process_lv) == -1) { // At wrong level
        printf("Manager Error\n");
      }
    }
  }

  // close quiz_grades.txt file
  fclose(file); 
  return 0;
}