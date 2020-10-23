#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>

struct timeval start;
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
//Common signal
// SIGKILL
// SIGCONT
// SIGSTOP
 
int state_1=0;
int state_2=0;

void handle_signal_34(int sig)
{
	struct timeval end;
	gettimeofday(&end,0);
	printf("Received request to connect. Sending approval!! -Server || %lf msec\n",timedifference_msec(start,end));
	// fflush(stdout);
	state_1=1;
}

void handle_signal_35(int sig)
{
	state_2=1;
}

void handle_signal_36(int sig)
{
	state_1=2;
	struct timeval end;
	gettimeofday(&end,0);
	printf("Connected!! Let't talk - Server || %lf msec\n",timedifference_msec(start,end));
	// fflush(stdout);
}

void process_1(int pid_child){
	// Assign custom handler for user defined signal
	// If process_1 receives signal # 34 it will call handle_signal_34
	//signal(signal,function)
	signal(SIGRTMIN+0, handle_signal_34);
	signal(SIGRTMIN+2, handle_signal_36);

	while(state_1==0);

	kill(pid_child,SIGRTMIN+1);
	//To prevent interuppting a lower prioirty signal can use sigqueue
	// sigqueue(pid_child, SIGRTMIN+1,(const union sigval)NULL);
	
	while(state_1==1);
	exit(0);
}
 
void process_2(){
	// Assign custom handler for user defined signal
	signal(SIGRTMIN+1, handle_signal_35);
 
	kill(getppid(),SIGRTMIN+0);
   	struct timeval end;
	gettimeofday(&end,0);
	printf("'I want to start connection' - Client ||  %lf msec\n",timedifference_msec(start,end));
	// fflush(stdout);

	// Timeout waiting for response
	sleep(5);
	if(state_2==1){
		kill(getppid(),SIGRTMIN+2);
		gettimeofday(&end,0);
		printf("'I received the approval to connect' Starting connection - Client || %lf msec\n",timedifference_msec(start,end));	
		// fflush(stdout);

	}
	exit(0);	
}
int main(int argc, char *argv[])
{
	gettimeofday(&start,0);
	int pid1=fork();
	if(pid1==0){
		int pid2 = fork();
		if(pid2==0)
			process_2(); 
		process_1(pid2);
	}
	return 0;
}