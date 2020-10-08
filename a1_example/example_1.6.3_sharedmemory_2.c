#include <stdio.h>
#include <sys/wait.h>  
#include <unistd.h>    
#include <string.h>  
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>  // for using shared memory 


int main(int argc, char**argv) {
	
	int num_integers = 3;
	int size_bytes = num_integers * sizeof(int);
	int shmid = shmget(IPC_PRIVATE, size_bytes, 0666 | IPC_CREAT);
	int *pShm= shmat(shmid, 0, 0);
	for(int i = 0; i <num_integers; i++){
		pShm[i] = i;
	}
	
	if(fork() == 0) {
		for(int i = 0; i <num_integers; i++){
			printf ("Child read: %d.\n",pShm[i]) ;
		}
		exit(0);
	}
	else {
		wait(NULL);
		for(int i = 0; i <num_integers; i++){
			printf ("Parent read: %d.\n",pShm[i]) ;
		}

   }

    shmdt(pShm);
    shmctl(shmid, IPC_RMID, NULL);
	printf ("Done.\n");
	  
	return 0;
}








