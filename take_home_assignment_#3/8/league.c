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

void process_manager(int n, int team, int shmid){
    int * msg = (int *)shmat(shmid, 0, 0);
    int is_home = 0;
    int home_goals, other_goals, home_team, other_team;

    while(1){
        gettimeofday(&end,0);
        // printf("%d: Stop\t\t\t\t\t\t%lf msec\n", team, timedifference_msec(start,end));
        raise(SIGSTOP);
        gettimeofday(&end,0);
        // printf("%d: Continue\t\t\t\t\t%lf msec\n", team, timedifference_msec(start,end));
        while(msg[4*0+0] == 0);
        while(msg[4*0+1] == 0);
        gettimeofday(&end,0);
        // printf("%d: Message recieved\t\t\t\t%lf msec\n", team, timedifference_msec(start,end));
        home_team = msg[4*0+0];
        other_team = msg[4*0+1];
        if(home_team == team){
            gettimeofday(&end,0);
            // printf("%d: Home collected match info\t\t\t%lf msec\n", team, timedifference_msec(start,end));
            msg[4*0+2] = 1;
        } else {
            gettimeofday(&end,0);
            // printf("%d: Other collected match info\t\t\t%lf msec\n", team, timedifference_msec(start,end));
            msg[4*0+3] = 1;
        }
        if(home_team == team){
            gettimeofday(&end,0);
            printf("%d: Starting match:\tTeam %d vs Team %d\t\t%lf msec\n", team, home_team, other_team, timedifference_msec(start,end));
        }
        sleep(3);
        if(home_team == team){
            gettimeofday(&end,0);
            printf("%d: Match ended:\t\tTeam %d vs Team %d\t\t%lf msec\t", team, home_team, other_team, timedifference_msec(start,end));
        }
        if(team == home_team){
            home_goals = rand()%6;
            other_goals = rand()%6;
            gettimeofday(&end,0);
            printf("Result:%d-%d\t%lf msec\n", home_goals, other_goals, timedifference_msec(start,end));
            if(home_goals > other_goals){
                msg[4*home_team+0]++;
                msg[4*other_team+2]++;
            } else if(home_goals < other_goals){
                msg[4*home_team+2]++;
                msg[4*other_team+0]++;
            } else {
                msg[4*home_team+1]++;
                msg[4*other_team+1]++;
            }
            msg[4*home_team+3] += home_goals;
            msg[4*other_team+3] += other_goals;
            // printf("\nTeam\tW\tD\tL\tGS\tPoints\n");
            // printf("--------------------------------------------------\n");
            // for(int i = 1; i<=n; i++){
            //     printf("%d\t%d\t%d\t%d\t%d\t%d\n", i, msg[4*i+0], msg[4*i+1], msg[4*i+2], msg[4*i+3], 3*msg[4*i+0]+msg[4*i+0]);
            // }
        }
    }

    shmdt(msg); // is this done on SIGTERM?
}

int main(int argc, char * argv[]){
    
    gettimeofday(&start,0);
    srand(time(NULL));

	int fd_in = open("input1.txt",O_RDONLY); 
	if(fd_in<0)
	{
		perror("file cant be opened");
		return 0;
	}
	dup2(fd_in,0);  //(stdin :0)

    int n;
    scanf("%d", &n);

	key_t key = ftok("/home/inorder_driver",68);
    // {0: {home_team, other_team, home_collected, other_collected}, 1-n: { W, D, L, GS}}
	int shmid = shmget(IPC_PRIVATE, 4*(n+1)*sizeof(int), 0666 | IPC_CREAT);

    int * manager_pid = (int *) malloc((n+1)*sizeof(int));

    for(int i = 1; i<=n; i++){
        manager_pid[i] = fork();
        if(manager_pid[i] == 0){
            process_manager(n, i, shmid);
            exit(0);
        }
    }

    int * msg = (int *)shmat(shmid, 0, 0);

    // simulate
    int line_count = n*(n-1);
    while(line_count--){
        int home_team, other_team;
        scanf("%d %d\n", &home_team, &other_team);
        gettimeofday(&end,0);
        // printf("Schedule: %d %d\t\t\t\t\t%lf msec\n", home_team, other_team, timedifference_msec(start,end));
        waitpid(manager_pid[home_team], NULL, WUNTRACED);
        waitpid(manager_pid[other_team], NULL, WUNTRACED);
        gettimeofday(&end,0);
        // printf("Sending start message: %d %d\t\t\t%lf msec\n", home_team, other_team, timedifference_msec(start,end));
        msg[4*0+0] = home_team;
        msg[4*0+1] = other_team;
        msg[4*0+2] = 0;
        msg[4*0+3] = 0;
        kill(manager_pid[home_team], SIGCONT);
        kill(manager_pid[other_team], SIGCONT);
        while(msg[4*0+2] == 0);
        while(msg[4*0+3] == 0);
        gettimeofday(&end,0);
        // printf("Message delivered to %d %d\t\t\t%lf msec\n", home_team, other_team, timedifference_msec(start,end));
        msg[4*0+0] = 0;
        msg[4*0+1] = 0;
    }

    sleep(4);

    gettimeofday(&end,0);
    // printf("Terminating all processes\t\t\t%lf msec\n", timedifference_msec(start,end));
    for(int i = 1; i<=n; i++){
        waitpid(manager_pid[i], NULL, WUNTRACED);
        kill(manager_pid[i], SIGKILL);
    }
    gettimeofday(&end,0);
    // printf("All processes terminated\t\t\t%lf msec\n", timedifference_msec(start,end));

    while(wait(NULL) != -1);

    printf("\nTeam\tW\tD\tL\tGS\tPoints\n");
    printf("--------------------------------------------------\n");
    for(int i = 1; i<=n; i++){
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", i, msg[4*i+0], msg[4*i+1], msg[4*i+2], msg[4*i+3], 3*msg[4*i+0]+msg[4*i+0]);
    }

    shmdt(msg);
    shmctl(shmid, IPC_RMID, NULL);
	close(fd_in);
    return 0;
}