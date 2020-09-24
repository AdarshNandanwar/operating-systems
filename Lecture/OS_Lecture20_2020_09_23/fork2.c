#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
int main(int argc, char *argv[])
{
	pid_t var1;
	int g1=100;
	var1=fork();
	if(var1<0)
	{
		perror("fork failed\n");
		return 0;
	}
	else if(var1==0)
	{
		g1+=200;
	}
	else
	{
		g1+=500;
		wait(NULL); 
	}
	printf("Process with PID=%d completed\n",getpid());
	printf("g1 value in %d = %d\n",getpid(),g1);
	return 0;
}
