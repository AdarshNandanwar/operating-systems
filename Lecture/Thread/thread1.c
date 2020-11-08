#include<pthread.h> 
#include<stdio.h>
int sum;
void *runner(void *param);
int main(int argc,char *argv[])
{ 
	printf("Main Thread:The pid is %d\n",getpid());	
	printf("Main thread:The tid is %u\n",pthread_self());	
	pthread_t tid;	
	pthread_attr_t attr; 
	pthread_attr_init(&attr); 
	pthread_create(&tid,&attr,runner,argv[1]);
	printf("The newly created thread ID = %u\n",tid); 
	pthread_join(tid,NULL); 
	printf("Main Thread: sum = %d \n",sum);
}

// runner function
void *runner ( void *param )
{ 	
	int upper=atoi(param);
 	int i; 
 	sum=0;
	printf("New Thread: The pid is %d\n",getpid());	
	printf("New Thread: The tid is %u\n",pthread_self());	
 	if (upper>0) 
	{
		for ( i=1; i <= upper; i++ ) 
	   	{ 
			sum = sum + i;
	   	}
	}
	printf("New Thread: sum = %d \n",sum);
	pthread_exit(0);
}
