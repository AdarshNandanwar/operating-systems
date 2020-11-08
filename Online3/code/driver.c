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

#include"cricket.h"

#define N 2048
#define READ_END 0
#define WRITE_END 1

struct timeval start;
struct timeval end;
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}


int main(int argc, char * argv[]){

    srand(time(NULL));
    gettimeofday(&start,0);

    fclose(fopen("commentary.txt", "w"));

/*
    // PRINT TIME

    gettimeofday(&end,0);
    printf("%lf msec\n", timedifference_msec(start,end));
*/


    // 1 = batting, 0 = bowling
    // [0:umpire, 1:team_1, 2:team_2, 3:bowler, 4:batsman_1, 5:batsman_2, 6:team_1_ready, 7:team_2_ready, 8:umpire_toss, 9:curr_strike, 10:team_1_task, 11:baller_seed, 12:batsman_seed, 
    //  13:balls bowled in that innings, 14:current_bowler, 15:current_batsman_1, 16:current_batsman_2, 17: match_over]
    key_t key = ftok("/home/driver",68);
    shmid = shmget(IPC_PRIVATE, N*sizeof(int), 0666 | IPC_CREAT);

    int * msg = (int *)shmat(shmid, 0, 0);

    msg[6] = 0;
    msg[7] = 0;
    msg[8] = -1;
    msg[9] = 1;
    msg[14] = 0;
    msg[15] = 0;
    msg[16] = 1;
    msg[17] = 0;

    int pid_team_1 = fork();
    if(pid_team_1 == 0){
        process_team(1);
        exit(0);
    }
    msg[1] = pid_team_1;

    int pid_team_2 = fork();
    if(pid_team_2 == 0){
        process_team(2);
        exit(0);
    }
    msg[2] = pid_team_2;

    int pid_umpire = fork();
    if(pid_umpire == 0){
        process_umpire();
        exit(0);
    }
    msg[0] = pid_umpire;

    shmdt(msg);

    shmctl(shmid, IPC_RMID, NULL);

    printf("Done!\n");

    wait(NULL);
    wait(NULL);
    wait(NULL);


    return 0;
}