#include<pthread.h> 
#include<stdio.h>
#include<asm/unistd.h>
int value1, value2;
void *runner(void *param);
void *fact(void *param);
int main(int argc,char *argv[])
{ 
	printf("Main Thread:TID self=%u, TID=%u, PID=%u\n",pthread_self(),syscall(__NR_gettid),getpid());
	pthread_t tid1, tid2;	
	pthread_attr_t attr; 
	pthread_attr_init(&attr); 
	pthread_create(&tid1,&attr,runner,argv[1]); 
	//pthread_create(&tid2,&attr,fact,argv[1]); 
	pthread_join(tid1,NULL); 
	//pthread_join(tid2,NULL); 
	printf("Main Thread: value1 = %d, value2 = %d\n",value1,value2);
}

// runner function
void *runner ( void *param )
{ 	
	int upper=atoi(param);
 	int i; 
	pthread_t tid2;	
 	value1=0;
	printf("New Thread:TID self=%u, TID=%u, PID=%u\n",pthread_self(),syscall(__NR_gettid),getpid());
 	if (upper>0) 
	{
		for ( i=1; i <= upper; i++ ) 
	   	{ 
			value1 = value1 + i;
	   	}
	}
	pthread_create(&tid2,NULL,fact,param);
	pthread_join(tid2,NULL); 
	printf("New Thread runner: value1 = %d and value2 = %d\n",value1,value2);
	pthread_exit(0);
}
void *fact ( void *param )
{ 	
	int upper=atoi(param);
 	int i; 
 	value2=0;
	printf("New Thread:TID self=%u, TID=%u, PID=%u\n",pthread_self(),syscall(__NR_gettid),getpid());
 	if (upper>=0) 
	{
		value2=1;
		for ( i=2; i <= upper; i++ ) 
	   	{ 
			value2 = value2 * i;
	   	}
	}
	printf("New Thread fact: value1 = %d and value2 = %d\n",value1,value2);
	exit(0);
	//pthread_exit(0);
}
