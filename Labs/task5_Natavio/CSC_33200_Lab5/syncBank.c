// Lab 5
// Mark Natavio
// Task 1
// The given bank.c program has 3 processes namely, the dad process and two son processes where a son withdrawing
// money from the bank and the dad depositing money in the bank, at randomly selected time intervals. The program
// in the given form has synchronization errors and logical errors. The program itself compiles correctly.

// In the bank there are also 2 atm machines. Meaning at a time there are only 2 processes that can request to 
// view the balance or can withdraw or deposit money.

// Identify the critical section and synchronize the program. You would need to use P(sem) and V(sem) operations at 
// the right places to solve the synchronization problem. You will also need to find the logical flaw if there is 
// any and try to solve it. To use P(sem) and V(sem) include the provided “sem.h” file. Your solution should solve 
// the following problems:
//    • Prevent race conditions [DONE]
//    • Prevent a son from withdrawing money when there is no balance. [DONE]
//    • Prevent undefined outputs like negative balance. [DONE]
//    • Prevent a process from continuously requesting access to the shared memory. Prevent unnecessary cpu cycle. [DONE]
//    • The problem should be solved with as few semaphore variables as possible. [DONE]

// My Changes:
// Single line changes and comments will contain a /// comment
// Multiple lines of code I add/cjhange will be encompassed by a // ********************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "sem.h"

#define CHILD      			 0  			/* Return value of child process from fork call */
#define TRUE       			 0  
#define FALSE      			 1

#define NumOfDepositAttempt  5           /* How many times Dad will deposit money */
#define NumOfWithdrawAttempt 20          /* Total number of attempts SON_1 and SON_2 has */

#define DepositAmount        60         /* The amound of money Dad deposits at a time */
#define WithdrawAmount       20         /* The amount of money Son withdraws at a time */
#define initBalance          100				/// Changed the initial balance from $0 to $100 as stated in line 66

FILE *fp1, *fp2, *fp3, *fp4;			/* File Pointers */

int main() {
	int pid;						// Process ID after fork call
	int i;							// Loop index
	int N;							// Number of times dad does update
	int N_Att;						// Number of time sons allowed to do update
	int status;						// Exit status of child process
	int bal1, bal2;					// Balance read by processes
	int flag, flag1;				// End of loop variables
	srand(time(NULL));

	// ********************************************************************************
	/// Setting the semaphores
	/// Semaphores are used to allow and deny access to a common resource amongst 
	/// processes. In this case, that common resource is the balance in the dad's bank
	/// account. So I created a semaphor that will be used to allow access to withdraw
	/// money from the bank acount. I formatted it just like in syncExample.c
	int semBank = semget(IPC_PRIVATE,1,0666 | IPC_CREAT);
	sem_create(semBank, 1);
	// ********************************************************************************

	//Initialize the file balance to be $100
	fp1 = fopen("balance.txt","w+");
	bal1 = initBalance; /// Logical error: the file starts with initBalance = 0 but the comment in line 66 states its 100
											/// so I changed initBalance to 100
	fprintf(fp1, "%d\n", bal1);
	fclose(fp1);
	
	//Initialize the number of withdraw attempts 
	fp4 = fopen("attempt.txt", "w+");
	N_Att = NumOfWithdrawAttempt;
	fprintf(fp4, "%d\n", N_Att);
	fclose(fp4);
	
	//Create child processes that will do the updates
	if ((pid = fork()) == -1) {
		//fork failed!
		perror("fork");
		exit(1);
	}
	
	if (pid == CHILD) {
	//First Child Process. Dad tries to do some updates.
	  printf("Dad's Pid: %d\n",getpid());
		N = NumOfDepositAttempt;
		for(i = 1; i <= N; i++) {
			// ********************************************************************************
			P(semBank); /// Calling P operation on semaphore at start of critical portion
			// ********************************************************************************
			printf("Dad is requesting to view the balance.\n"); //Dad is requesting to get hold of an ATM.
			fp1 = fopen("balance.txt", "r+"); //Dad successfully got hold of the ATM.
			fscanf(fp1, "%d", &bal2);
			printf("Dad reads balance = %d \n", bal2);
		  int r = rand()%5 + 1;
			printf("Dad needs %d sec to prepare money\n", r);

			// ********************************************************************************
			V(semBank); /// Calling V operation on semaphore to allow other processes to work
			sleep(r); //Dad Process is sleeping for r sec. You need to make sure that other processes can work in the mean time.
			P(semBank); /// Calling P operation on semaphore to allow other porcesses to work
			
			fp1 = fopen("balance.txt", "r+"); /// Get new balance in case it was changed while dad Process prepared money
			fscanf(fp1, "%d", &bal2);
			// ********************************************************************************

			fseek(fp1, 0L, 0); //Dad will now deposit the money. For this Dad will access the ATM again. And update the current balance.
			bal2 += DepositAmount;
			fprintf(fp1, "%d \n", bal2);
			fclose(fp1);
			printf("Dad writes new balance = %d \n", bal2);
			printf("Dad will deposit %d more time\n", N-i); //Dad depostited the money. 
			// ********************************************************************************
			V(semBank); /// Calling V operation on semaphore at end of critical portion
			// ********************************************************************************
			sleep(rand()%10 + 1);	/* Dad will wait some time for requesting to see balance again.*/
		}
	}
	
	else { 
		//Parent Process. Fork off another child process.
		if ((pid = fork()) == -1) {
			//Fork failed!
			perror("fork");
			exit(1);
		}
		if (pid == CHILD) {
			printf("First Son's Pid: %d\n",getpid());
			//Second child process. First poor son tries to do updates.
			flag = FALSE;
			while(flag == FALSE) {
				// ********************************************************************************
				P(semBank); /// Calling P operation on semaphore at start of critical portion
				// ********************************************************************************
				printf("SON_1 is requesting to view the balance.\n"); //Son_1 is requesting to get hold of the ATM.
				fp3 = fopen("attempt.txt" , "r+"); //Son_1 successfully got hold of the ATM.
				fscanf(fp3, "%d", &N_Att); // Son_1 Checks if he has more than 0 attempt remaining.
				printf("Attempt remaining: %d.\n", N_Att);
				if(N_Att == 0) {
					fclose(fp3);
					flag = TRUE;
				}
				else {
					fp2 = fopen("balance.txt", "r+");//Son_1 reads the balance. 
					fscanf(fp2,"%d", &bal2);
					printf("SON_1 reads balance. Available Balance: %d \n", bal2);
					printf("SON_1 wants to withdraw money. "); //And if balance is greater than Withdraw amount, then son can withdraw money.

					// ********************************************************************************
					/// Check if there is enough balance for son 1 to withdraw $20
					if (bal2 >= WithdrawAmount) { /// if there is enough balance, withdraw money
						fseek(fp2,0L, 0);
						bal2 -=WithdrawAmount;
						fprintf(fp2,"%d\n", bal2);
						fclose(fp2);
						printf("SON_1 withdrawed %d. New Balance: %d \n",WithdrawAmount, bal2);
					} else { /// If not enough balance, state there isn't enough balance 
						printf("\nNot enough balance to withdraw\n");
					}
					// ********************************************************************************

					fseek(fp3,0L, 0); //SON_1 will write the number of  attempt remaining in the attampt.txt file.
					N_Att -=1;
					fprintf(fp3, "%d\n", N_Att);
					fclose(fp3);
					printf("Number of attempts remaining:%d \n", N_Att);

				}
				// ********************************************************************************
				V(semBank); /// Calling V operation on semaphore at end of critical portion
				// ********************************************************************************
				sleep(rand()%10+1); //SON_1 will wait some time before the next request.
			}
		}
		else {
			
		//Parent Process. Fork off one more child process.
			if ((pid = fork()) == -1) {
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD) {
				printf("Second Son's Pid: %d\n",getpid()); 
				//Third child process. Second poor son tries to do updates.
				flag1 = FALSE;
				while(flag1 == FALSE) {
					// ********************************************************************************
					P(semBank); /// Calling P operation on semaphore at start of critical portion
					// ********************************************************************************
					printf("SON_2 is requesting to view the balance.\n"); //Son_2 is requesting to get hold of the ATM.
					fp3 = fopen("attempt.txt" , "r+"); //Son_2 successfully got hold of the ATM.
					fscanf(fp3, "%d", &N_Att); // Son_2 Checks if he has more than 0 attempt remaining.
					printf("Attempt remaining: %d.\n", N_Att);
					if(N_Att == 0) {
						fclose(fp3);
						flag1 = TRUE;
					}
					else {
						
						fp2 = fopen("balance.txt", "r+"); //Son_2 reads the balance. 
						fscanf(fp2,"%d", &bal2);
						printf("SON_2 reads balance. Available Balance: %d \n", bal2);
						printf("SON_2 wants to withdraw money. "); //And if balance is greater than Withdraw amount, then son can withdraw money.
						
						// ********************************************************************************
						/// Check if there is enough balance for son 2 to withdraw $20
						if (bal2 >= WithdrawAmount) { /// if there is enough balance, withdraw money
							fseek(fp2,0L, 0);
							bal2 -=WithdrawAmount;
							fprintf(fp2,"%d\n", bal2);
							fclose(fp2);
							printf("SON_2 withdrawed %d. New Balance: %d \n",WithdrawAmount, bal2);
						} else { /// If not enough balance, state there isn't enough balance 
							printf("Not enough balance to withdraw\n");
						}
						// ********************************************************************************
						
						fseek(fp3,0L, 0); //SON_2 will write the number of  attempt remaining in the attampt.txt file.
						N_Att -=1;
						fprintf(fp3, "%d\n", N_Att);
						fclose(fp3);
						printf("Number of attempts remaining: %d \n", N_Att);
						
					}
					// ********************************************************************************
					V(semBank); /// Calling V operation on semaphore at end of critical portion
					// ********************************************************************************
					sleep(rand()%10+1);//SON_2 will wait some time before the next request.
				}
			}
			else {
				
				//Now parent process waits for the child processes to finish
				
				pid = wait(&status);
              	printf("child(pid = %d) exited with the status %d. \n", pid, status);

              	pid = wait(&status);
              	printf("child(pid = %d) exited with the status %d. \n", pid, status);

              	pid = wait(&status);
              	printf("child(pid = %d) exited with the status %d. \n", pid, status);
			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}




