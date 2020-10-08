#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h> // for using kill()
#include <sys/shm.h>  // for using shared memory



int main(int argc, char* argv[])
{
    printf("This is the BEGINNING of the program.\n\n");
    
    if(argc-1 != 3){
        printf("Error: The number of input integers now is %d. Please input 3 integers.\n",argc-1);
        return -1;
    }// Don't modify this Error Checking part

    else{
        // Write your code in this brace
        int shmid;
        int counter;
        int *shmc_B, *shmc_A, *shmp;
        int size_data = 3 * sizeof(int);
        int differ[3] = {atoi(argv[1]), atoi(argv[2]), atoi(argv[3])};

        shmid = shmget(IPC_PRIVATE, size_data, 0666 |IPC_CREAT);
        pid_t fpid_B = fork();

        if(fpid_B < 0)
            printf("Error in executing fork!");

        else if(fpid_B == 0){ // Child process B
            // sleep(1);
            shmc_B = shmat(shmid, 0, 0);  // Attach the program to the memory.

            printf("Receive a SIGCONT.\n");
            printf("Child B process ID: %d.\n",getpid());

            int sum = *(shmc_B) + *(shmc_B+1);
            // printf("the sum value is : %d\n",sum);
            if(sum > 0)
                printf("The 3rd argument is larger than the 1st argument.\n\n");
            else if(sum < 0){
                printf("The 3rd argument is smaller than the 1st argument.\n\n");
            }
            else{
                printf("The 3rd argument is equal to the 1st argument.\n\n");
            }
            shmdt(shmc_B);                // Detach the program from the memory

            _exit(0);

        }

        else { // Parent process

            kill(fpid_B, SIGSTOP); // send signal to stop the Child process B
            pid_t fpid_A = fork(); // fork A

            if (fpid_A < 0)
                printf("Error in executing fork!");

            else if (fpid_A == 0) { // Child process A
                shmc_A = shmat(shmid, 0, 0);  // Attach the program to the memory.

                printf("Receive a SIGCONT.\n");
                printf("Child A process ID: %d.\n", getpid());
                //int sum = *(shmc_B) + *(shmc_B+1);
                printf("Sum of differences: %d.\n", *(shmc_A) + *(shmc_A + 1));
                printf("Send a SIGCONT to process %d.\n\n", fpid_B);
                kill(fpid_B, SIGCONT);
                shmdt(shmc_A);           // Detach the program from the memory
                _exit(0);

            } else { // Parent Process

                kill(fpid_A, SIGSTOP); // send signal to stop the Child process A
                shmp = shmat(shmid, 0, 0); // Attach the program to the memory.

                for (int i = 0; i < 2; i++) { // do the calculation of subtraction
                    *(shmp + i) = differ[i + 1] - differ[i];
                }

                printf("Apply %d bytes.\n", size_data);
                printf("Parent process ID: %d.\n", getpid());
                printf("Differences: %d, %d.\n", *(shmp), *(shmp + 1));
                printf("Send a SIGCONT to process %d.\n\n", fpid_A);
                kill(fpid_A, SIGCONT); // send signal to Child process. tell it to continue.
                while(wait(NULL) >0);
                // Wait until the All child process A exited.
                counter++;
                printf("Exited Process ID: %d; Count: %d.\n", fpid_A, counter);
                counter++;
                printf("Exited Process ID: %d; Count: %d.\n", fpid_B, counter);
                shmdt(shmp);
                shmctl(shmid, IPC_RMID, NULL);
            }
        }
    }
    printf("This is the END of the program.\n");
    return 0;
}


// Please use the following lines to print related information if needed.

/*
sleep(1);  // which you may need for registration of signal handlers in child process,
               //only allowd at the beginning of the parent process after fork();

printf("Apply %d bytes.\n",***);
printf("Child process A ID: %d.\n", getpid());
printf("Child process B ID: %d.\n", getpid());
printf("Sum of differences: %d.\n\n", ***);
printf("Send a SIGCONT to Process %d.\n\n", ***);
printf("Parent process ID: %d.\n");
printf("Differences: %d, %d.\n", ***,***);
printf("Send a SIGCONT to process %d.\n\n", ***);
printf("Exited Process ID: %d.\n", **);
printf("Receive a SIGCONT.\n");
printf("Receive a SIGSTOP.\n");


printf("Exited Process ID: %d; Count: %d.\n", **, ***);
printf("The 3rd argument is larger than the 1st argument.\n");
printf("The 3rd argument is smaller than the 1st argument.\n");
printf("The 3rd argument is equal to the 1st argument.\n");
*/



