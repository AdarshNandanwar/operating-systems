#include<pthread.h> 
#include<stdio.h>
#include<stdlib.h>
int *fib;
void *runner(void *param);
int main(int argc,char *argv[])
{
	int val,N,i; 
	pthread_t tid;	
	pthread_attr_t attr; 
	pthread_attr_init(&attr);
	pthread_attr_setinheritsched(&attr,PTHREAD_INHERIT_SCHED); 	
	pthread_attr_getdetachstate(&attr,&val);
	if(val!= PTHREAD_CREATE_JOINABLE)
 		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	pthread_create(&tid,&attr,runner,argv[1]);
	pthread_join(tid,NULL);
	N=atoi(argv[1]);
	for(i=0;i<N;i++)
		printf("fib[%d] in Main Thread = %d \n",i,fib[i]);
	free(fib);
	return 0;	
}

// runner function
void *runner ( void *param )
{ 	
	struct sched_param sparam; 
	int i,N;
	sparam.sched_priority=10;
	pthread_setschedparam(pthread_self(),SCHED_RR,&sparam);
	pthread_detach(pthread_self());
	N = atoi(param);
	if(N>0)
	{
		fib=(int *)malloc(N*sizeof(int));
		if(N>1)fib[0]=0;
		if(N>2)fib[1]=1;
		for(i=2;i<N;i++)
			fib[i]=fib[i-1]+fib[i-2];
		for(i=0;i<N;i++)
			printf("fib[%d] in New Thread = %d \n",i,fib[i]);
	}
	pthread_exit(0);
}
