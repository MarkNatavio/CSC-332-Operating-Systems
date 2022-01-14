// Lab 6
// Mark Natavio
// This is my pthread implementation of the Cigarette Smoker's Problem

// Problem:
// There are four processes in this problem: three smoker processes and an agent process. Each
// of the smoker processes will make a cigarette and smoke it. To make a cigarette requires
// tobacco, paper, and matches. Each smoker process has one of the three items.

// I.e., one process has tobacco, another has paper, and a third has matches. The agent has an 
// infinite supply of all three. The agent places two of the three items on the table, and the 
// smoker that has the third item makes the cigarette. Synchronize the processes.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h> 
#include <sys/wait.h>	
#include <sys/types.h>	

// Defining processes
void *agent(); // define agent function
void *smokers(void *arg); // define smoker function

// Defnining pthread values
pthread_t agent_pthread, tobacco_pthread, paper_pthread, match_pthread;

// Defining the mutex values
pthread_mutex_t lock_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t agent_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tobacco_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t paper_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t match_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
  srand(time(0)); // ensure randomization

  // locking and unlocking the mutex objects
  pthread_mutex_unlock(&lock_mutex);
  pthread_mutex_lock(&agent_mutex);
  pthread_mutex_lock(&tobacco_mutex);
  pthread_mutex_lock(&paper_mutex);
  pthread_mutex_lock(&match_mutex);
  
  // creating new threads for each process
  pthread_create(&agent_pthread, NULL, agent, NULL);
  pthread_create(&match_pthread, NULL, smokers, (void *)1); // match pthread id = 1
  pthread_create(&paper_pthread, NULL, smokers, (void *)2); // paper pthread id = 2
  pthread_create(&tobacco_pthread, NULL, smokers, (void *)3); // tobacco pthread id = 3
  
  pthread_join(agent_pthread, NULL);
  
  // destroy mutex values after code is finished
  pthread_mutex_destroy(&agent_mutex);
  pthread_mutex_destroy(&tobacco_mutex);
  pthread_mutex_destroy(&paper_mutex);
  pthread_mutex_destroy(&match_mutex);
  pthread_mutex_destroy(&lock_mutex);
  return 0;
}

// Function to define agent process
void *agent(){
  int counter = 0; // remove this to be more truthful to the problem

  while (counter < 10){ // while (1) { // replace with this to be more truthful to the problem
    pthread_mutex_lock(&lock_mutex); // lock the lock_pthread
    int randNum = (rand() % 3) + 1; // Pick a random number from 1-3

    // Check which process was randomly chosen
    if (randNum == 1) { // Matches smoker was chosen
      printf("\nAgent places tobacco on the table\n");
      printf("Agent places paper on the table.\n");
      pthread_mutex_unlock(&match_mutex); // Wake up smoker with match
    } else if (randNum == 2) { // Paper smoker was chosen
      printf("\nAgent places tobacco on the table\n");
      printf("Agent places match on the table.\n");
      pthread_mutex_unlock(&paper_mutex); // Wake up smoker with paper
    } else { // Tobacco smoker was chosen
      printf("\nAgent places match on the table\n");
      printf("Agent places paper on the table.\n");
      pthread_mutex_unlock(&tobacco_mutex); // Wake up smoker with tobacco
    }

    pthread_mutex_unlock(&lock_mutex);
    pthread_mutex_lock(&agent_mutex); // Agent sleeps
    counter += 1;
  }

  pthread_mutex_lock(&lock_mutex);

  // sends a cancellation request to a thread
  pthread_cancel(paper_pthread);
  pthread_cancel(tobacco_pthread);
  pthread_cancel(match_pthread);
  pthread_mutex_unlock(&lock_mutex); // unlock the lock
  return 0;
}

void *smokers(void *arg){
  int ID = (int) arg; // get ID of chosen process
  
  if (ID == 1) { // if ID = 1, matches smoker was chosen
    while(1){
      pthread_mutex_lock(&match_mutex);
      pthread_mutex_lock(&lock_mutex);
      printf("Matches smoker picks up tobacco from the table.\n");
      printf("Matches smoker picks up paper from the table.\n");
      printf("Matches smoker makes a cigarette and goes to sleep.\n"); // Smoke (but don't inhale).
      pthread_mutex_unlock(&agent_mutex); // unlock the agent
      pthread_mutex_unlock(&lock_mutex); // unlock the lock
    }
    
  } else if (ID == 2) { // if ID = 2, paper smoker was chosen
    while(1){
      pthread_mutex_lock(&paper_mutex);  // Sleep right away
      pthread_mutex_lock(&lock_mutex);
      printf("Paper smoker picks up tobacco from the table.\n");
      printf("Paper smoker picks up matches from the table.\n");
      printf("Paper smoker makes a cigarette and goes to sleep.\n"); // Smoke (but don't inhale).
      pthread_mutex_unlock(&agent_mutex); // unlock the agent
      pthread_mutex_unlock(&lock_mutex); // unlock the lock
    }

  } else { // if ID = 3, tobacco smoker was chosen
    while(1) {
      pthread_mutex_lock(&tobacco_mutex);  // Sleep right away
      pthread_mutex_lock(&lock_mutex);
      printf("Tobacco smoker picks up matches from the table.\n");
      printf("Tobacco smoker picks up paper from the table.\n");
      printf("Tobacco smoker makes a cigarette and goes to sleep.\n"); // Smoke (but don't inhale).
      pthread_mutex_unlock(&agent_mutex); // unlock the agent
      pthread_mutex_unlock(&lock_mutex); // unlock the lock
    }
  }
  return 0;
}