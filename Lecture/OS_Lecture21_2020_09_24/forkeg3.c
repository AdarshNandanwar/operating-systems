#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
	int i;
	printf("fork program starting with pid %d\n",getpid());
	for(i=0;i<3;i++)
		fork();
	printf("My PID = %d and My PPID = %d\n",getpid(),getppid());
	while(wait(NULL)!=-1); 
	return 0;
}