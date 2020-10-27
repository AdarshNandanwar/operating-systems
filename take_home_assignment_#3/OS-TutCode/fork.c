#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 	
#include <sys/time.h>
#include <sys/wait.h>

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
void writeFile(FILE* ptr , char data,int len){
	for(int i=0;i<len;i++)
		fputc(data,ptr);
	fclose(ptr);
	exit(0);
}

int main()
{
	char data= 'A';
	// int len = 5;
	int len = 50000000;
	struct timeval start;
   	struct timeval end;
   	///////////////////////////
	gettimeofday(&start, 0); 
	FILE *ptrA=fopen("A.txt","w");
	FILE *ptrB=fopen("B.txt","w");
	for(int i=0;i<len;i++)
		fputc(data,ptrA);
	for(int i=0;i<len;i++)
		fputc(data,ptrB);
	fclose(ptrA);
	fclose(ptrB);
	gettimeofday(&end,0);
	printf("Time by single process for 2 files is  %lf miliseconds\n",timedifference_msec(start,end));
	fflush(stdout);
   	///////////////////////////
	gettimeofday(&start, 0); 
	int pid=fork();
	if(pid==0){
		printf("Process started with pid %d\n", getpid());
		FILE *ptrC=fopen("C.txt","w");
		writeFile(ptrC,data,len);
	}
	pid=fork();
	if(pid==0){
		printf("Process started with pid %d\n", getpid());
		FILE *ptrD=fopen("D.txt","w");
		writeFile(ptrD,data,len);
	}
	while(wait(NULL)!=-1);
	gettimeofday(&end,0);
	printf("Time by multi process for 2 files is  %lf miliseconds\n",timedifference_msec(start,end));
	////////////////////////////
}