#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
int main(int argc, char *argv)
{
	pid_t p1;
	int retValue,retStat;
	printf("Parent process PID =%d\n",getpid());
	p1=fork();
	if(p1<0)
	{
		perror("fork failed\n");
		return 0;
	}
	else if(p1==0)
	{
		printf("Child process with PID=%d is executing\n",getpid());
		printf("The p1 value in %d PID process is %d my parent is %d\n",getpid(),p1,getppid()); 
		exit(1);
	}
	else
	{
		printf("Parent process with PID=%d is executing\n",getpid());
		printf("The p1 value in %d PID process is %d\n",getpid(),p1);
		retValue=wait(&retStat);
		printf("Return status of child process is %d\n",retStat/256);
		printf("return value of wait is %d\n",retValue); 
	}
	printf("Process with PID=%d completed\n",getpid());
	return 3;
}