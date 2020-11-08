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
#define READ_END 0
#define WRITE_END 1

struct timeval start;
struct timeval end;
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}


void get_cwd(char * cwd){
    int pipe_cwd[2];
    if(pipe(pipe_cwd) == -1){
        printf("Couldn't create pipe for pwd.\n");
        return;
    }
    if(fork() == 0){
        dup2(pipe_cwd[WRITE_END], STDOUT_FILENO);
        close(pipe_cwd[READ_END]);
        close(pipe_cwd[WRITE_END]);
        execlp("pwd", "pwd", (char *)NULL);
    } else {
        waitpid(-1, NULL, 0);
        close(pipe_cwd[WRITE_END]);
        read(pipe_cwd[READ_END], cwd, N);
        close(pipe_cwd[READ_END]);
    }

    cwd[strlen(cwd)-1] = '\0';
}


int main(int argc, char * argv[]){
    
    srand(time(NULL));
    gettimeofday(&start,0);

/*
    // PRINT TIME

    gettimeofday(&end,0);
    printf("%lf msec\n", timedifference_msec(start,end));
*/


/*
    // FILE INPUT

	int fd_in = open("input.txt",O_RDONLY); 
	if(fd_in<0)
	{
		perror("file cant be opened");
		return 0;
	}
	dup2(fd_in,0);  //(stdin :0)


	close(fd_in);
*/


/*
    // SHARED MEMORY

	key_t key = ftok("/home/template",68);
	int shmid = shmget(IPC_PRIVATE, N*sizeof(int), 0666 | IPC_CREAT);

    int * msg = (int *)shmat(shmid, 0, 0);
    shmdt(msg);

    shmctl(shmid, IPC_RMID, NULL);
*/


/*
    // PIPE

    int pipe_1[2];
    if(pipe(pipe_1) == -1){
        printf("Couldn't create pipe.\n");
        return -1;
    }
    if(fork() == 0){
        dup2(pipe_1[WRITE_END], STDOUT_FILENO);
        close(pipe_1[READ_END]);
        close(pipe_1[WRITE_END]);
        execl("/bin/pwd", "pwd", (char *)NULL);
    }
    else {
        waitpid(-1, NULL, 0);
        close(pipe_1[WRITE_END]);
        char exec_output[1000];
        read(pipe_1[READ_END], exec_output, 1000);
        close(pipe_1[READ_END]);
        printf("%s", exec_output);
    }
*/


/*
    // NESTED PIPE

    int pipe_1[2];
    if(pipe(pipe_1) == -1){
        printf("Couldn't create pipe\n");
        return;
    }

    if(fork() == 0){
        // child
        dup2(pipe_1[WRITE_END], STDOUT_FILENO);
        close(pipe_1[READ_END]);
        close(pipe_1[WRITE_END]);
        execl("/bin/ls", "ls", "-F", (char *)NULL);
    } else {
        // parent
        waitpid(-1, NULL, 0);
        close(pipe_1[WRITE_END]);

        // CHECKING USING grep
        int pipe_2[2];
        if(pipe(pipe_2) == -1){
            printf("Couldn't create pipe\n");
            return;
        }

        if(fork() == 0){
            // child
            dup2(pipe_1[READ_END], STDIN_FILENO);
            dup2(pipe_2[WRITE_END], STDOUT_FILENO);
            close(pipe_2[READ_END]);
            close(pipe_2[WRITE_END]);
            execl("/bin/grep", "grep", "/$", (char *)NULL);
        } else {
            // parent
            waitpid(-1, NULL, 0);
            close(pipe_2[WRITE_END]);
            char exec_output[1000];
            read(pipe_2[READ_END], exec_output, 1000);
            printf("%s", exec_output);
            close(pipe_2[READ_END]);
        }
        close(pipe_1[READ_END]);
    }
*/


/*
    // GET CURRENT WORKING DIRECTORY (CWD)

    char * cwd = (char *) malloc(N*sizeof(char));
    get_cwd(cwd);

*/

    return 0;
}