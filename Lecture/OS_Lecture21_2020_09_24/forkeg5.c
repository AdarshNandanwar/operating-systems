#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
int main(int argc, char *argv[])
{
	int i;
	printf("fork program starting with pid %d\n",getpid());
	for(i=0;i<2;i++)
	{
		if(fork())
		{
			if(!fork())
			{
				fork();
				printf("My PID = %d and My PPID = %d\n",getpid(),getppid());
				exit(0);
			}
		}
	}
	printf("My PID = %d and My PPID = %d\n",getpid(),getppid());
	while(wait(NULL)!=-1); 
	return 0;
}