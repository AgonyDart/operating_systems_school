#include <stdio.h>
#include <sys/types.h>

int getppid();
int getpid();
int wait();
int sleep();
void exit();
pid_t fork();

int main()
{
	for(int i=0;i<5;i++) // loop will run n times (n=5)
	{
		if(fork() == 0)
		{
			printf("[hijo] pid %d from [padre] pid %d\n",getpid(),getppid());
            sleep(10);
			exit(0);
		}
	}
	for(int i=0;i<5;i++) // loop will run n times (n=5)
	wait(NULL);
	
}