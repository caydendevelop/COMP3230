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
  
int main(int argc, char* argv[]) 
{
	printf("This is the BEGINNING of the program.\n\n");
	
	{
		// Write your program in this brace
		if(argc-1 != 3){
			printf("Error: The number of input integers now is %d. Please input 3 integers.\n",argc-1);
			return -1;
		}

		int shmid;
		int *shm;

		int size_data = 3 * sizeof(int);
		printf ("Apply %d bytes.\n",size_data);

		shmid = shmget(IPC_PRIVATE, size_data * sizeof(int), 0666 | IPC_CREAT);
		shm = shmat(shmid, 0, 0);
		
		int pid1; 
		pid1 = fork();
		if(pid1 ==0){ 
			signal(SIGCONT, sigcontin); 
			kill(getpid(), SIGSTOP);

			printf("Child process ID: %d.\n", getpid());
			
			// Calculate the sum
			shm[2] = shm[0] + shm[1];
			printf("Sum of differences: %d.\n\n", shm[2]);
						
			exit(0);
		}
		else{
			sleep(1);

			shm[0] =  atoi(argv[2])- atoi(argv[1]);
			shm[1] =  atoi(argv[3])- atoi(argv[2]);
			printf("Parent process ID: %d.\nDifferences: %d, %d.\n", getpid(), shm[0], shm[1]);
			
			printf("Send a SIGCONT to process %d.\n\n", pid1); 
			kill(pid1, SIGCONT);
			
			int count = 0;
			int p_exit1 = wait(NULL);
			printf("Exited Process ID: %d.\n", p_exit1); 

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
  






