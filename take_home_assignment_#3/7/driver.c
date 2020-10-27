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

struct timeval start;
struct timeval end;
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

void search(int ** maze, int r, int c, int n){
    // printf("[%d, %d] ", r, c);
    if(r == n-1 && c == n-1){
        printf("(%d, %d) -> ", r, c);
        exit(1);
    } 
    int pid_right = -2, pid_down = -2, status = 0;
    if(maze[r][c] == 0){
        if(r < n-1){
            pid_right = fork();
            if(pid_right == 0){
                search(maze, r+1, c, n);
                exit(0); // not really needed
            }
        }
        if(c < n-1){
            pid_down = fork();
            if(pid_down == 0){
                search(maze, r, c+1, n);
                exit(0); // not really needed
            }
        }
    }
    if(pid_right>0){
        waitpid(pid_right, &status, 0);
        if(status) {
            printf("(%d, %d) -> ", r, c);
            exit(1);
        }
    }
    if(pid_down>0){
        waitpid(pid_down, &status, 0);
        if(status) {
            printf("(%d, %d) -> ", r, c);
            exit(1);
        }
    }
    exit(0);
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

    int n;
    scanf("%d", &n);
    
    int ** maze = (int **) malloc(n*sizeof(int *));
    for(int i = 0; i<n; i++) maze[i] = (int *) malloc(n*sizeof(int));
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            scanf("%d ", &maze[i][j]);
        }
    }

    if(fork() == 0){
        search(maze, 0, 0, n);
    }
    
    int status;
    wait(&status);
    if(status == 0){
        printf("Spiderman cannot rescue MJ.");
    }
    printf("\n");

	close(fd_in);
    return 0;
}