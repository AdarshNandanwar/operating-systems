#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<asm/unistd.h>


void *runner(void *param);
int main(int argc,char *argv[])
{ 	
	pthread_t tid,tid1,tid2;	
	pthread_attr_t attr; 
	pthread_attr_init(&attr); 
	pthread_create(&tid,&attr,runner,argv[1]);
	printf("The first created thread is with id %u\n",tid);
	pthread_create(&tid1,&attr,runner,argv[2]); 
/*	printf("The second created thread is with id %u\n",tid1);
	pthread_create(&tid2,&attr,runner,argv[3]); 
	printf("The third created thread is with id %u\n",tid2);
	if(!fork())
	{
		printf("In Child Process: My PID = %d and my parent PID = %d\n",getpid(),getppid()); 
		printf("From Child Process: The TID is %d\n",syscall(__NR_gettid));
	}
	else
	{
		wait(NULL);
*/		printf("In Parent Process: My PID = %d and my parent PID = %d\n",getpid(),getppid()); 
		printf("From Parent Process: The TID is %d\n",syscall(__NR_gettid));

//	}
	pthread_join(tid,NULL); 
//	pthread_join(tid1,NULL);
//	pthread_join(tid2,NULL);
	return 0;
}

// runner function
void *runner ( void *param )
{ 	
	int upper=atoi(param);
 	int i; 
	int sum=0;
 	if (upper>0) 
	{
		for ( i=1; i <= upper; i++ ) 
	   	{ 
			sum = sum + i;
	   	}
	}
	fork();
	printf("From thread: The final result in thread %u is %d\n",pthread_self(),sum);
	printf("From thread: My PID = %d and my parent PID = %d\n",getpid(),getppid()); 
	printf("From thread: The TID is %d\n",syscall(__NR_gettid));
	pthread_exit(0);
}
