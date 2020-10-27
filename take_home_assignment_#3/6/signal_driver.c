#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#define N 1024

// idle: 0, playing = 1
int state;

void handle_home_ground(int sig){
    while(state == 1);
    state = 1;
    sleep(3);
    
}

void handle_other_ground(int sig){
    
}

void process_manager(){
    state = 0;

    signal(SIGRTMIN+0, handle_home_ground);
    signal(SIGRTMIN+1, handle_other_ground);

    while(1) sleep(100);
}

int main(int argc, char * argv[]){
    
	int fd_in = open("input.txt",O_RDONLY); 
	if(fd_in<0)
	{
		perror("file cant be opened");
		return 0;
	}
	dup2(fd_in,0);  //(stdin :0)

    int n;
    scanf("%d", &n);

    int * manager_pid = (int *) malloc(n*sizeof(int));

    for(int i = 0; i<n; i++){
        manager_pid[0] = fork();
        if(manager_pid[0] == 0){
            process_manager();
            exit(0);
        }
    }

    // simulate
    int line_count = n*(n-1);
    while(line_count--){
        int t1, t2;
        scanf("%d %d\n", &t1, &t2);
        sigqueue(manager_pid[t1-1], SIGRTMIN+0,(const union sigval)NULL);
        sigqueue(manager_pid[t2-1], SIGRTMIN+1,(const union sigval)NULL);
        printf("%d %d\n", t1, t2);
    }

    for(int i = 0; i<n; i++){
        kill(manager_pid[i], SIGTERM);
    }

    while(wait(NULL) != -1);

	close(fd_in);
    return 0;
}