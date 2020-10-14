#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>  // for using shared memory


int main() {
//    int shmid, shmid_1;
//    int *shmp, *shmp_1, *shmc, *shmc_1;
    //int arr[8] = {0,1,2,3,4,5,6,7};

//    int size_data = 1 * sizeof(int);
//    printf ("Apply %d bytes.\n",size_data);

//    shmid = shmget(IPC_PRIVATE, size_data, 0666 | IPC_CREAT);
//    shmid_1 = shmget(IPC_PRIVATE, size_data, 0666 | IPC_CREAT);
    pid_t pid;

    for(int i = 0; i<2; i++)
        pid = fork();

        if (pid == 0) {
//            shmc = shmat(shmid, NULL, 0);
//            shmc_1 = shmat(shmid_1, NULL, 0);
//            *shmc = *shmc + 4;
//            *shmc_1 = *shmc_1 + 8;
//            printf ("Child: shmc is %d and shmc_1 is %d.\n",*shmc, *shmc_1) ;
            printf ("Child: pid is %d and parent is %d.\n", getpid(), getppid()) ;
//
//            shmdt(shmc);
//            shmdt(shmc_1);
            //exit(0);

        } else {
            while(wait(NULL)>0);
//            shmp = shmat(shmid, NULL, 0);
//            shmp_1 = shmat(shmid_1, NULL, 0);
//            printf("Parent: shmc is %d and shmc_1 is %d.\n", *shmp, *shmp_1);
//            shmdt(shmp);
//            shmdt(shmp_1);
//            shmctl(shmid, IPC_RMID, NULL);
//            shmctl(shmid_1, IPC_RMID, NULL);
            printf ("finished\n") ;
            //exit(0);
        }

    return 0;
}








