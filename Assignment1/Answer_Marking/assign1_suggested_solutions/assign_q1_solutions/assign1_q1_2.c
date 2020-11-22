#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h>

#include <sys/wait.h> 


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>  // for using shared memory
  
void sigcontin(int sig); // function declaration 
void sigstop(int sig);
  
int main(int argc, char* argv[]) 
{
	printf("This is the BEGINNING of the program.\n\n");
	
	if(argc-1 != 3){
		printf("Error: The number of input integers now is %d. Please input 3 integers.\n",argc-1);
		return -1;
	}

	int shmid;
	//int *shmp, *shmc1, *shmc2;
	int *shm;

	int size_data = 3 * sizeof(int);
	printf ("Apply %d bytes.\n",size_data);

	shmid = shmget(IPC_PRIVATE, size_data * sizeof(int), 0666 | IPC_CREAT);
	shm = shmat(shmid, 0, 0);
	
	int pid1, pid2; 
	pid1 = fork();
	if (pid1 < 0) { 
		exit(-1); 
	} 
	else if (pid1 == 0) { /* child */
		signal(SIGCONT, sigcontin); 		
		kill(getpid(), SIGSTOP);
		printf("Child process B ID: %d.\n", getpid());
		
		if(shm[2] > 0){
			printf("The 3rd argument is larger than the 1st argument.\n");
		}
		else if (shm[2] < 0){
			printf("The 3rd argument is smaller than the 1st argument.\n");
		}
		else{
			printf("The 3rd argument is equal to the 1st argument.\n");
		}	
		printf("\n");

		exit(0);
	} 
	else { /* parent */
		pid2 = fork();
		if(pid2 ==0){ 
			signal(SIGCONT, sigcontin); 		
			kill(getpid(), SIGSTOP);
			printf("Child process A ID: %d.\n", getpid());
			
			// Calculate the sum
			shm[2] = shm[0] + shm[1];
			printf("Sum of differences: %d.\n", shm[2]);
			
			// Send the signal			
			printf("Send a SIGCONT to process %d.\n\n", pid1); 
			kill(pid1, SIGCONT);
			
			exit(0);
		}
		else{
			sleep(1);

			shm[0] =  atoi(argv[2])- atoi(argv[1]);
			shm[1] =  atoi(argv[3])- atoi(argv[2]);
			printf("Parent process ID: %d.\nDifferences: %d, %d.\n", getpid(), shm[0], shm[1]);


			printf("Send a SIGCONT to process %d.\n\n", pid2); 
			kill(pid2, SIGCONT);
			
			int count = 0;
			int p_exit1 = wait(NULL);
			printf("Exited Process ID: %d; Count: %d.\n", p_exit1, ++count); 

			int p_exit2 = wait(NULL);
			printf("Exited Process ID: %d; Count: %d.\n", p_exit2, ++count); 
			shmdt(shm);
			shmctl(shmid, IPC_RMID, NULL);

		}
	}
	
	printf("This is the END of the program.\n");
	return 0;
} 
  
void sigcontin(int sig){ 
	printf("Received a SIGCONT.\n"); 
	signal(SIGCONT, SIG_DFL); /* reset signal */
} 







