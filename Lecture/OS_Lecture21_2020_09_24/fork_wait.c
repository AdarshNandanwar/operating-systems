
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/types.h>
int main(int argc, char *argv[])
{
	pid_t var1;
	int status, termret;
	var1=fork();
	if(var1<0)
	{
		perror("fork failed\n");
		exit(1);
	}
	else if(var1==0)
	{
		printf("I am in Child process with PID %d, I am exiting\n",getpid());
		exit(3);
	}
	else
	{
		printf("I am in Parent process with PID %d\n",getpid());
	}	 
	termret=wait(&status); 
//	termret=waitpid(var1,&status,0); 
	printf("Child with PID %d exited, exit status is %d\n",termret,(status/256));
	return 0;
}