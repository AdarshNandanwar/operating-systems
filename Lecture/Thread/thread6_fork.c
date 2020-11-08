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
	pthread_create(&tid1,&attr,runner,argv[2]); 
	printf("thread IDs are %u\t %u\t\n",tid,tid1);
	if(fork())
		printf("Parent Pro: PID=%d, PPID=%d\n",getpid(),getppid());
	else
		printf("Child Pro: PID=%d, PPID=%d\n",getpid(),getppid());
	pthread_create(&tid2,&attr,runner,argv[3]); 
	pthread_join(tid,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	wait(NULL);

}

// runner function
void *runner ( void *param )
{ 	
	int upper=atoi(param);
 	int i; 
//	int sum=0;
 	if (upper>0) 
	{
		for ( i=1; i <= upper; i++ ) 
	   	{ 
			sum = sum + i;
	   	}
	}
	printf("From thread:PID=%d,PID_TID=%d, TID=%u and SUM=%d\n",getpid(),syscall(__NR_gettid),pthread_self(),sum);
	pthread_exit(0);
}
