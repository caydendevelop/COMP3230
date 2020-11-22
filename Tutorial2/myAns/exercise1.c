#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	printf("This is the beginning of the program!\n\n");
	
	pid_t fpid;   //fpid, return value of fork function
	
	fpid = fork();
	
    if (fpid < 0){
		printf("Error in executing fork!");
        _exit(0);
    }
	else if (fpid == 0)
	{
		pid_t cpid;
		cpid = fork();
        if (cpid < 0){
			printf("Error in executing fork!");
            _exit(0);
        }
		else if (cpid == 0)
		{
			printf("The exited child process ID now is %d, whose parent process ID is %d \n", getpid(), getppid());
            _exit(0);
		}
		else
		{
			wait(NULL);
            printf("The exited child process ID now is %d, whose parent process ID is %d \n", getpid(), getppid());
            _exit(0);
		}
		
	}
	else 
	{	
		wait(NULL);
		pid_t ppid;
		ppid = fork();
        if (ppid < 0){
			printf("Error in executing fork!");
            _exit(0);
        }
		else if (ppid == 0)
		{
            printf("The exited child process ID now is %d, whose parent process ID is %d \n", getpid(), getppid());
            _exit(0);
		}
		else
		{
			wait(NULL);
			printf("The final exited process ID now is %d.\n", getpid());
		}
		
	}

	
	return 0;
}
