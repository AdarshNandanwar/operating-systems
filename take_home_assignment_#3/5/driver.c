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

struct timeval start;
struct timeval end;
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

int * pid_chef;
int * parent;
int n, cur_chef, parent_res;

void handle_signal_34(int sig){

    int is_parent_done = (parent_res != 0);
    int child_res = 0, status;

    for(int i = 0; i<n; i++){
        if(parent[i] == cur_chef){
            kill(pid_chef[i], SIGRTMIN+0);
            waitpid(pid_chef[i], &status, 0);
            child_res += status/256;
        }
    }
    gettimeofday(&end,0);
    if(is_parent_done){
        printf("Chef #%d->\t#dishes made: %d, #dishes collected: %d\t\t%lf msec\n", cur_chef+1, parent_res, child_res, timedifference_msec(start,end));
        exit(parent_res+child_res);
    } else {
        printf("Chef #%d->\t#dishes made: %d, #dishes collected: %d\t\t%lf msec\n", cur_chef+1, 0, 0, timedifference_msec(start,end));
        exit(0);
    }
}

void simulate(int * V, int * T){
    // printf("pid: %d (%d), ppid: %d\n", getpid(), cur_chef+1, getppid());

    signal(SIGRTMIN+0, handle_signal_34);

    for(int i = 0; i<n; i++){
        if(parent[i] == cur_chef){
            pid_chef[i] = fork();
            if(pid_chef[i] == 0){
                cur_chef = i;
                simulate(V, T);
                exit(0);
            }
        }
    }

    sleep(T[cur_chef]);
    parent_res = V[cur_chef];

    if(cur_chef == 0){
        kill(getpid(), SIGRTMIN+0);
    }else {
        while(1) sleep(100);
    }
}

int main(int argc, char * argv[]){
 
    gettimeofday(&start,0);
    
	int fd_in = open("input1.txt",O_RDONLY); 
	if(fd_in<0)
	{
		perror("file cant be opened");
		return 0;
	}
	dup2(fd_in,0);  //(stdin :0)

    int a, b;
    scanf("%d", &n);
    
    parent = (int *) malloc(n*sizeof(int));
    for(int i = 0; i<n; i++) parent[i] = -1;
    for(int i = 0; i<n-1; i++){
        scanf("%d %d\n", &a, &b);
        parent[b-1] = a-1;
    }

    int * V = (int *) malloc(n*sizeof(int));
    for(int i = 0; i<n; i++){
        scanf("%d ", &V[i]);
    }
    int * T = (int *) malloc(n*sizeof(int));
    for(int i = 0; i<n; i++){
        scanf("%d ", &T[i]);
    }

    pid_chef = (int *) malloc(n*sizeof(int));

    parent_res = 0;
    cur_chef = 0;
    if(fork() == 0){
        simulate(V, T);
    }

    int status;
    wait(&status);
    printf("Total Worthy Dishes: %d\n", status/256);

	close(fd_in);
    return 0;
}