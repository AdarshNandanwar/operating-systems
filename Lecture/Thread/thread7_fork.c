#include<pthread.h> 
#include<stdio.h>
#include<asm/unistd.h>
void *runner(void *param);
int sum=0;
int main(int argc,char *argv[])
{ 	
	pthread_t tid,tid1,tid2;	
	pthread_attr_t attr; 
	pthread_attr_init(&attr); 
	pthread_create(&tid,&attr,runner,argv[1]);
	tid=pthread_join(tid,NULL);
	printf("From Main thread: sum value is %d\n",sum);
	return 0;
}

// runner function
void *runner ( void *param )
{ 	
	int upper=atoi(param);
 	int i; 
 	if (upper>0) 
	{
		for ( i=1; i <= upper; i++ ) 
	   	{ 
			sum = sum + i;
	   	}
	}
	/*if(fork())
	{
		sum=sum+10;
		printf("From thread:PID=%d,PID_TID=%d, TID=%u and SUM=%d\n",getpid(),syscall(__NR_gettid),pthread_self(),sum);
		printf("Parent process: pid = %d and ppid = %d\n",getpid(),getppid());
	}
	else
	{
		sum=sum+30;
		printf("From thread:PID=%d,PID_TID=%d, TID=%u and SUM=%d\n",getpid(),syscall(__NR_gettid),pthread_self(),sum);
		printf("Child process: pid = %d and ppid = %d\n",getpid(),getppid());
	}*/
	if(!fork())
	{
		execl("/bin/ls","ls",0);
		printf("Exec failed\n");
	}
	wait(NULL);
	pthread_exit(0);
}
