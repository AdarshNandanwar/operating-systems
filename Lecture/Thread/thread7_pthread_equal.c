#include<pthread.h> 
#include<stdio.h>

int sum;
void *runner(void *param);
int main(int argc,char *argv[])
{ 
	sum=0;
	printf("Main Thread:The Sum = %d\t The pid is %d\t The tid is %u\n",sum,getpid(),pthread_self());	
	pthread_t tid,tid1,tid2;	
	pthread_attr_t attr; 
	pthread_attr_init(&attr); 
	pthread_create(&tid,&attr,runner,argv[1]); 
	pthread_create(&tid1,&attr,runner,argv[1]); 
	tid2=tid1;
	if(pthread_equal(tid,tid1))
		printf("tid and tid1 refers to the same thread\n");
	else
		printf("tid and tid1 refers to different threads\n");
	if(pthread_equal(tid2,tid1))
		printf("TID1 and TID2 refers to the same thread\n");
	else
		printf("TID1 and TID2 refers to different threads\n");
	pthread_join(tid,NULL); 
	pthread_join(tid1,NULL); 
	printf("Main Thread: sum = %d \n",sum);
}

// runner function
void *runner ( void *param )
{ 	
	int upper=atoi(param);
 	int i; 
	printf("New Thread: The pid is %d\t The tid is %u\n",getpid(),pthread_self());	
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
