// Lab 6
// Mark Natavio
// This is my sempahore implementation of the Cigarette Smoker's Problem

// Problem:
// There are four processes in this problem: three smoker processes and an agent process. Each
// of the smoker processes will make a cigarette and smoke it. To make a cigarette requires
// tobacco, paper, and matches. Each smoker process has one of the three items.

// I.e., one process has tobacco, another has paper, and a third has matches. The agent has an 
// infinite supply of all three. The agent places two of the three items on the table, and the 
// smoker that has the third item makes the cigarette. Synchronize the processes.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include "sem.h"

int main() {
  // Setting up variables
  int pid, status;
  int counter = 0; // remove this to be more truthful to the problem

  // Setting up the semaphors
  int lock_sem = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
  int agent_sem = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
  int tobacco_sem = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
  int paper_sem = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
  int matches_sem = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);

  // Initializing semaphors
  // All semaphores except for lock_sem are initialized to 0. "lock is initialized to 1"
  sem_create(lock_sem, 1);
  sem_create(agent_sem, 0);
  sem_create(tobacco_sem, 0);
  sem_create(paper_sem, 0);
  sem_create(matches_sem, 0);

  // Create child processes that will do the updates
  if ((pid = fork()) == -1) {
		//fork failed!
		perror("fork");
		exit(1);
	}
  
  //First Child Process. The agent smoker process
  if (pid == 0) { 
    while (counter < 10) { // while (1) { // replace with this to be more truthful to the problem
      P(lock_sem); // lock sem sleeps
      int randNum = (rand() % 3) + 1; // Pick a random number from 1-3
      if (randNum == 1) { // if random number = 1, put tobacco and paper on table
        printf("\nAgent places tobacco on the table\n");
        printf("Agent places paper on the table.\n");
        V(matches_sem); // Wake up smoker with match
      } else if (randNum == 2) {
        printf("\nAgent places tobacco on the table\n");
        printf("Agent places match on the table.\n");
        V(paper_sem); // Wake up smoker with paper
      } else {
        printf("\nAgent places match on the table.\n");
        printf("Agent places paper on the table.\n");
        V(tobacco_sem); // Wake up smoker with tobacco
      }
      V(lock_sem); // lock sem wakes up
      P(agent_sem); // Agent sleeps
      counter += 1;
    }
  } else {

    // Parent Process. Fork off another child process.
    if ((pid = fork()) == -1) {
      //fork failed!
      perror("fork");
      exit(1);
    }
  
    if (pid == 0) { // Second child process. Tobacco smoker
      while(1) {
        P(tobacco_sem);  // tobacco sem sleeps right away
        P(lock_sem); // lock sem sleeps
        printf("Tobacco smoker picks up matches from the table.\n");
        printf("Tobacco smoker picks up paper from the table.\n");
        V(agent_sem); // wake the agent
        V(lock_sem); // wake the lock sem
        printf("Tobacco smoker makes a cigarette and goes to sleep.\n"); // Smoke (but don't inhale).
      }
    } else {

      // Parent Process. Fork off another child process.
      if ((pid = fork()) == -1) {
        //fork failed!
        perror("fork");
        exit(1);
      }
    
      if (pid == 0) { // Third child process. Paper smoker
        while(1) {
          P(paper_sem);  // Sleep right away
          P(lock_sem); // lock sem sleeps
          printf("Paper smoker picks up tobacco from the table.\n");
          printf("Paper smoker picks up matches from the table.\n");
          V(agent_sem); // wake the agent
          V(lock_sem); // wake the lock sem
          printf("Paper smoker makes a cigarette and goes to sleep.\n"); // Smoke (but don't inhale).
        }
      } else {

        // Parent Process. Fork off another child process.
        if ((pid = fork()) == -1) {
          //fork failed!
          perror("fork");
          exit(1);
        }
      
        if (pid == 0) { // Fourth child porcess. Matches smoker
          while(1){
            P(matches_sem);  // Sleep right away
            P(lock_sem); // lock sem sleeps
            printf("Matches smoker picks up tobacco from the table.\n");
            printf("Matches smoker picks up paper from the table.\n");
            V(agent_sem); // wake the agent
            V(lock_sem); // wake the lock sem
            printf("Matches smoker makes a cigarette and goes to sleep.\n"); // Smoke (but don't inhale).
          }
        } else {
          
          // Process ends
          pid = wait(&status);
					printf("\nProcess with pid = %d exited with the status %d. \n", pid, status);
        }
      }
    }
  }
}
