#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(void) {
    for(int i = 0; i < 2; i++) {
        pid_t pid = fork();

        if(pid == 0){  // child process
            printf("I am Child. Parent Process ID is %d, My ID is %d \n", getppid(), getpid());
            // _exit(0);
        }
        else{  // Parent process
            wait(NULL);
            printf("I am Parent. My ID is %d \n", getpid());
        }

    }

    return 0;
}
