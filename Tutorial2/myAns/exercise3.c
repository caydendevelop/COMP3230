#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>  // for using shared memory


int main( int argc, char *argv[] )  {
    
    argc = argc - 1;
    
    if (argc!=3)
        printf("Error: The number of input integers now is %d. Please input 3 integers.", argc);
    
    else{
        int shmid;
        int *shmc, *shmp;
        int size_data = 3 * sizeof(int);
        int arr[3] = {atoi(argv[1]), atoi(argv[2]), atoi(argv[3])};
        shmid = shmget(IPC_PRIVATE, size_data, 0666 |IPC_CREAT);
        pid_t fpid = fork();

        if(fpid < 0)
            printf("Error in executing fork!");

        else if(fpid == 0){ // Child process, doing sort
            shmc = shmat(shmid, 0, 0);  // Attach the program to the memory.
            
            for(int i =0; i<3; i++){
              *(shmc+i) = arr[i];
            }
            
            int i, j;
            
            for (i = 0; i < 3; i++){
                for (j = 0; j < 3-i-1; j++){
                    if (*(shmc+j) > *(shmc+j+1)){
                        int temp;
                        temp = *(shmc+j);
                        *(shmc+j) = *(shmc+j+1);
                        *(shmc+j+1) = temp;
                    }
                }
            }
            printf("Child process ID: %d; Sorting results: %d, %d, %d. \n\n", getpid(), *shmc, *(shmc+1), *(shmc+2));
            //printf ("Parent process ID: %d; Calculate the sum of the two smallest arguments: %d, %d.\n",getppid(), *(shmc), *(shmc+1));
            shmdt(shmc);                              // Detach the program from the memory
            
        }

        else {
          wait(NULL);
            shmp = shmat(shmid, 0, 0);
            printf ("Parent process ID: %d; Calculate the sum of the two smallest arguments: %d, %d.\n",getpid(), *(shmp), *(shmp+1));
            
            char str_1[9];
            char str_2[9];
            char str_3[9];
            sprintf(str_1, "%d", *(shmp));
            sprintf(str_2, "%d", *(shmp+1));
            sprintf(str_3, "%d", *(shmp+2));





            char *arg_vec[]={"./para_sum",str_1,str_2,str_3, NULL};
            execv("./para_sum", arg_vec);


            shmdt(shmp);
            shmctl(shmid, IPC_RMID, NULL);
        }
        
    }
    return 0;
}
