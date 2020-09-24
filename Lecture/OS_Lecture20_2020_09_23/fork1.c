#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
int main(int argc, char *argv[])
{
	pid_t var1;
	printf("Program started. Process PID =%d\n",getpid());
	var1=fork();
	if(var1<0)
	{
		perror("fork failed\n");
		return 0;
	}
	else if(var1==0)
	{
		printf("Child process with PID=%d is executing\n",getpid());
		printf("The var1 value in %d PID process is %d my parent is %d\n",getpid(),var1,getppid()); 
	}
	else
	{
		printf("Process with PID=%d is executing\n",getpid());
		printf("The var1 value in %d PID process is %d\n",getpid(),var1);
		wait(NULL); 
	}
	printf("Process with PID=%d completed\n",getpid());
	return 0;
}
