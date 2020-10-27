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
#define N 1024

void create_tree(int number_of_processes, int levels, int index, int shmid){
    // printf("pid: %d, ppid: %d, number of processes: %d, levels: %d, index: %d\n", getpid(), getppid(), number_of_processes, levels, index);
    if(number_of_processes == 0 || levels == 0) return;
    int pid;

    // void *shmat(int shmid, const void *shmaddr, int shmflg);  
    // ( shmflg: SHM_RDONLY, SHM_EXEC)

    int * pid_child = (int *) malloc(number_of_processes*sizeof(int));

    for(int i = 0; i<number_of_processes; i++){
        pid = fork();
        if(pid == 0){
            int * order = (int *)shmat(shmid,(void *)0,0);
            order[0]++;
            int shm_index = order[0]-1;
            order[2*shm_index+1] = getpid();
            order[2*shm_index+2] = getppid();
            // printf("child created and stopped %d\n", getpid());
            shmdt(order);
            kill(getpid(), SIGSTOP);
            // printf("child resumed %d\n", getpid());
            create_tree((index*(index+1))/2+i+1, levels-1, (index*(index+1))/2+i, shmid);
            exit(0);
        } else {
            pid_child[i] = pid;
        }
    }

    for(int i = 0; i<number_of_processes; i++){
        waitpid(pid_child[i], NULL, WUNTRACED);
    }

    for(int i = 0; i<number_of_processes; i++){
        kill(pid_child[i], SIGCONT);
    }

    while(wait(NULL) != -1);
}

int main(int argc, char * argv[]){
    if(argc != 3){
        printf("Please give parameter in the format: <number_of_processes> <number of levels>\n");
        return -1;
    }
    
	// key_t ftok(const char *pathname, int proj_id);
	key_t key = ftok("/2/driver.c",68);

	// int shmget(key_t key, size_t size, int shmflg);
	int shmid = shmget(IPC_PRIVATE, N*sizeof(int), 0666 | IPC_CREAT);

    int * order = (int *)shmat(shmid,(void *)0,0);
    order[0] = 0;
    shmdt(order);

    create_tree(atoi(argv[1]), atoi(argv[2]), 0, shmid);

    order = (int *)shmat(shmid,(void *)0,0);
    for(int i = order[0]-1; i>=0; i--){
        printf("pid: %d, ppid: %d\n", order[2*i+1], order[2*i+2]);
    }

    shmdt(order);

	// int shmctl(int shmid, int cmd, struct shmid_ds *buf);
	shmctl(shmid, IPC_RMID, NULL);

    return 0;
}