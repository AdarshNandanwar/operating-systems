#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
	printf("fork program starting with pid %d\n",getpid());
	if(fork())
		if(!fork())
			fork();
	printf("My PID = %d and My PPID = %d\n",getpid(),getppid());
	wait(NULL); 
	wait(NULL); 
	return 0;
}