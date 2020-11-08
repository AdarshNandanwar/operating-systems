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

#define N 1024
#define READ_END 0
#define WRITE_END 1

int cur_team;
int * team_players_seed;
char ** team_players;

char * team_name1;
char * team_name2;
char ** team_player1;
char ** team_player2;

void handle_shot_hit(int sig){
    int * msg = (int *)shmat(shmid, 0, 0);
    srand(msg[11]+msg[12]+msg[13]);
    int res = rand()%8;


    FILE * fd_in = fopen("commentary.txt", "a");
    if(fd_in == NULL){
        printf("error opening \"input.txt\" file\n");
        return;
    }
    // if(res == 7){
    //     fprintf(fd_in, "%d bowled and got a wicket: %d\n", msg[14], msg[9] == 1 ? msg[15] : msg[16]);
    // } else {
    //     fprintf(fd_in, "%d bowled and %d scored %d\n", msg[14], msg[9] == 1 ? msg[15] : msg[16], res);
    // }
    if(msg[9] == 1){
        if(res == 7){
            fprintf(fd_in, "%s bowled and got a wicket: %s\n", team_player2[msg[14]], msg[9] == 1 ? team_player1[msg[15]] : team_player1[msg[16]]);
        } else {
            fprintf(fd_in, "%s bowled and %s scored %d\n", team_player2[msg[14]], msg[9] == 1 ? team_player1[msg[15]] : team_player1[msg[16]], res);
        }
    } else {
        if(res == 7){
            fprintf(fd_in, "%s bowled and got a wicket: %s\n", team_player1[msg[14]], msg[9] == 1 ? team_player2[msg[15]] : team_player2[msg[16]]);
        } else {
            fprintf(fd_in, "%s bowled and %s scored %d\n", team_player1[msg[14]], msg[9] == 1 ? team_player2[msg[15]] : team_player2[msg[16]], res);
        }
    }
    fclose(fd_in);

    
    printf("Umpire: sig -> %d, %d\n", msg[3], SIGRTMIN+res);
    sigqueue(msg[3], SIGRTMIN+res, (const union sigval) NULL);
    if(msg[9] == 1){
        printf("Umpire: sig -> %d, %d\n", msg[4], SIGRTMIN+res);
        sigqueue(msg[4], SIGRTMIN+res, (const union sigval) NULL);
    } else {
        printf("Umpire: sig -> %d, %d\n", msg[5], SIGRTMIN+res);
        sigqueue(msg[5], SIGRTMIN+res, (const union sigval) NULL);
    }
    shmdt(msg);
}

void process_umpire(){
    signal(SIGRTMIN+0, handle_shot_hit);







    char ** tp1;
    char ** tp2;

    int fd_in1;
    int fd_in2;
    fd_in1 = open("team1.txt", O_RDONLY); 
    if(fd_in1<0) {
        perror("file cant be opened");
        return;
    }

    dup2(fd_in1,0);  //(stdin :0)

    int seed_val;
    team_name1 = (char *) malloc(N*sizeof(char));
    team_name2 = (char *) malloc(N*sizeof(char));
    team_player1 = (char **) malloc(N*sizeof(char *));
    team_player2 = (char **) malloc(N*sizeof(char *));

    scanf("%s", team_name1);
    scanf("%d", &seed_val);

    for(int i = 0; i<11; i++){
        team_player1[i] = (char *) malloc(N*sizeof(char));
        scanf("%s", team_player1[i]);
    }

    close(fd_in1);


    fd_in2 = open("team2.txt", O_RDONLY); 
    if(fd_in2<0) {
        perror("file cant be opened");
        return;
    }
    dup2(fd_in2,0);  //(stdin :0)

    scanf("%s", team_name2);
    scanf("%d", &seed_val);

    for(int i = 0; i<11; i++){
        team_player2[i] = (char *) malloc(N*sizeof(char));
        scanf("%s", team_player2[i]);
    }

    close(fd_in2);







    int fd_in = open("umpire.txt", O_RDONLY); 
    if(fd_in<0) {
        perror("file cant be opened");
        return;
    }

    dup2(fd_in,0);  //(stdin :0)

    int seed_umpire, max_overs;
    scanf("%d", &seed_umpire);
    scanf("%d", &max_overs);

    close(fd_in);
    
    int * msg = (int *)shmat(shmid, 0, 0);

    printf("Umpire: Ready: max_overs %d\n", max_overs);




    // waiting for team ready
    while(msg[6] == 0 || msg[7] == 0);
    // [0:umpire, 1:team_1, 2:team_2, 3:bowler, 4:batsman_1, 5:batsman_2, 6:team_1_ready, 7:team_2_ready, 8:umpire_toss, 9:curr_strike, 10:team_1_task, 11:baller_seed, 12:batsman_seed, 
    //  13:balls bowled in that innings, 14:current_bowler, 15:current_batsman_1, 16:current_batsman_2]
    printf("[0: umpire: %d, 1:team_1: %d, 2:team_2: %d, 3:bowler: %d, 4:batsman_1: %d, 5:batsman_2: %d, 6:team_1_ready: %d, 7:team_2_rady: %d, 8:umpire_toss: %d, 9:curr_strike: %d, 10:team_1_task: %d, 11:baller_seed: %d, 12:batsman_seed: %d, 13:balls bowled in that innings: %d, 14:current_bowler: %d, 15:current_batsman_1: %d, 16:current_batsman_2: %d]\n", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6], msg[7], msg[8], msg[9], msg[10], msg[11], msg[12], msg[13], msg[14], msg[15], msg[16]);
    printf("Umpire: Teams ready:\n");
    msg[6] = 0;
    msg[7] = 0;
    


    srand(seed_umpire);
    int toss = rand()%2;
    msg[8] = toss;
    msg[10] = toss;

    fflush(stdout);
    printf("Umpire:  toss: %d\n", toss);


    // waiting for toss response
    while(msg[6] == 0 || msg[7] == 0);
    // [0:umpire, 1:team_1, 2:team_2, 3:bowler, 4:batsman_1, 5:batsman_2, 6:team_1_ready, 7:team_2_ready, 8:umpire_toss, 9:curr_strike, 10:team_1_task, 11:baller_seed, 12:batsman_seed, 
    //  13:balls bowled in that innings, 14:current_bowler, 15:current_batsman_1, 16:current_batsman_2]
    printf("[0: umpire: %d, 1:team_1: %d, 2:team_2: %d, 3:bowler: %d, 4:batsman_1: %d, 5:batsman_2: %d, 6:team_1_ready: %d, 7:team_2_ready: %d, 8:umpire_toss: %d, 9:curr_strike: %d, 10:team_1_task: %d, 11:baller_seed: %d, 12:batsman_seed: %d, 13:balls bowled in that innings: %d, 14:current_bowler: %d, 15:current_batsman_1: %d, 16:current_batsman_2: %d]\n", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6], msg[7], msg[8], msg[9], msg[10], msg[11], msg[12], msg[13], msg[14], msg[15], msg[16]);


    int current_over = 1, current_balls = 0, innings = 1;
    // balls bowled in that innings = msg[13];


    while(1){
        if(msg[6] == 1 && msg[7] == 1){
            msg[6] = 0;
            msg[7] = 0;

            int cur_bat = msg[15];
            if(msg[9] == 2){
                cur_bat = 16;
            }

            if(cur_bat >= 11){
                msg[17] = 1;
                exit(0);
            }

            // over, inning logic


            // [0:umpire, 1:team_1, 2:team_2, 3:bowler, 4:batsman_1, 5:batsman_2, 6:team_1_ready, 7:team_2_ready, 8:umpire_toss, 9:curr_strike, 10:team_1_task, 11:baller_seed, 12:batsman_seed, 
            //  13:balls bowled in that innings, 14:current_bowler, 15:current_batsman_1, 16:current_batsman_2]
            // printf("[0: umpire: %d, 1:team_1: %d, 2:team_2: %d, 3:bowler: %d, 4:batsman_1: %d, 5:batsman_2: %d, 6:team_1_ready: %d, 7:team_2_ready: %d, 8:umpire_toss: %d, 9:curr_strike: %d, 10:team_1_task: %d, 11:baller_seed: %d, 12:batsman_seed: %d, 13:balls bowled in that innings: %d, 14:current_bowler: %d, 15:current_batsman_1: %d, 16:current_batsman_2: %d]\n", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6], msg[7], msg[8], msg[9], msg[10], msg[11], msg[12], msg[13], msg[14], msg[15], msg[16]);

            printf("Umpire: overs: %d, inning %d\n", current_over, innings);

            printf("Umpire: next ball. sig -> %d, %d\n", msg[3], SIGRTMIN+8);
            sigqueue(msg[3], SIGRTMIN+8, (const union sigval) NULL);
            msg[13]++;
            current_balls++;
            if(current_balls == 6){
                current_balls = 0;
                current_over++;
                // printf("Umpire: over. sig -> %d, %d\n", msg[1], SIGRTMIN+0);
                // sigqueue(msg[1], SIGRTMIN+0, (const union sigval) NULL);
                // printf("Umpire: over. sig -> %d, %d\n", msg[2], SIGRTMIN+0);
                // sigqueue(msg[2], SIGRTMIN+0, (const union sigval) NULL);
            }
            if(current_over > max_overs){
                // change innings
                if(innings == 2){
                    // exit game
                    msg[17] = 1;
                    exit(0);
                } else {
                    innings = 2;
                    current_over = 1;
                    current_balls = 0;
                    // sigqueue(msg[1], SIGRTMIN+1, (const union sigval) NULL);
                    // sigqueue(msg[2], SIGRTMIN+1, (const union sigval) NULL);
                }
            }
        }
    };

    shmdt(msg);
}

// BATSMAN

void handle_ball_bowled(int sig){
    int * msg = (int *)shmat(shmid, 0, 0);

    printf("batsman: shot hit. sig -> %d, %d\n", msg[0], SIGRTMIN+0);
    

    sigqueue(msg[0], SIGRTMIN+0, (const union sigval) NULL);
    shmdt(msg);
}

void handle_bat_helper(int run){
    int * msg = (int *)shmat(shmid, 0, 0);

    // do stuff

    printf("batsman: run %d\n", run);


    if(msg[10] == 1){
        msg[6] = 1;
    } else {
        msg[7] = 1;
    }
    shmdt(msg);
}
void handle_bat_wicket(int sig){
    int * msg = (int *)shmat(shmid, 0, 0);

    // do stuff

    printf("batsman: wicket\n");
    srand(time(NULL));
    int cur_player = msg[15], other_player = msg[16];
    int max_p = cur_player;
    if(cur_player < other_player){
        max_p = other_player;
    }
    max_p++;
    msg[15] = max_p;



    if(msg[10] == 1){
        msg[6] = 1;
    } else {
        msg[7] = 1;
    }
    shmdt(msg);
}
void handle_bat_0(int sig){handle_bat_helper(0);}
void handle_bat_1(int sig){handle_bat_helper(1);}
void handle_bat_2(int sig){handle_bat_helper(2);}
void handle_bat_3(int sig){handle_bat_helper(3);}
void handle_bat_4(int sig){handle_bat_helper(4);}
void handle_bat_5(int sig){handle_bat_helper(5);}
void handle_bat_6(int sig){handle_bat_helper(6);}

void process_batsman(){
    signal(SIGRTMIN+0, handle_bat_0);
    signal(SIGRTMIN+1, handle_bat_1);
    signal(SIGRTMIN+2, handle_bat_2);
    signal(SIGRTMIN+3, handle_bat_3);
    signal(SIGRTMIN+4, handle_bat_4);
    signal(SIGRTMIN+5, handle_bat_5);
    signal(SIGRTMIN+6, handle_bat_6);
    signal(SIGRTMIN+7, handle_bat_wicket);
    signal(SIGRTMIN+8, handle_ball_bowled);

    int * msg = (int *)shmat(shmid, 0, 0);
    while(msg[17] == 0);
    shmdt(msg);
    exit(0);
}


// BALLER

void handle_ball_next(int sig){
    printf("baller: ball bowled\n");
    int * msg = (int *)shmat(shmid, 0, 0);


    if(msg[9] == 1){
        sigqueue(msg[4], SIGRTMIN+8, (const union sigval) NULL);
    } else {
        sigqueue(msg[5], SIGRTMIN+8, (const union sigval) NULL);
    }
    
    shmdt(msg);
}

void handle_ball_helper(int run){
    int * msg = (int *)shmat(shmid, 0, 0);

    // do stuff

    printf("baller: run %d\n", run);


    if(msg[10] == 0){
        msg[6] = 1;
    } else {
        msg[7] = 1;
    }
    shmdt(msg);
}
void handle_ball_wicket(int sig){
    int * msg = (int *)shmat(shmid, 0, 0);

    // do stuff



    if(msg[10] == 0){
        msg[6] = 1;
    } else {
        msg[7] = 1;
    }
    shmdt(msg);
}
void handle_ball_0(int sig){handle_ball_helper(0);}
void handle_ball_1(int sig){handle_ball_helper(1);}
void handle_ball_2(int sig){handle_ball_helper(2);}
void handle_ball_3(int sig){handle_ball_helper(3);}
void handle_ball_4(int sig){handle_ball_helper(4);}
void handle_ball_5(int sig){handle_ball_helper(5);}
void handle_ball_6(int sig){handle_ball_helper(6);}

void process_bowler(){
    signal(SIGRTMIN+0, handle_ball_0);
    signal(SIGRTMIN+1, handle_ball_1);
    signal(SIGRTMIN+2, handle_ball_2);
    signal(SIGRTMIN+3, handle_ball_3);
    signal(SIGRTMIN+4, handle_ball_4);
    signal(SIGRTMIN+5, handle_ball_5);
    signal(SIGRTMIN+6, handle_ball_6);
    signal(SIGRTMIN+7, handle_ball_wicket);
    signal(SIGRTMIN+8, handle_ball_next);

    printf("Baller: sleeping\n");

    int * msg = (int *)shmat(shmid, 0, 0);
    while(msg[17] == 0);
    shmdt(msg);
    exit(0);
}

// TEAM

void handle_next_over(int sig){
    //
    int * msg = (int *)shmat(shmid, 0, 0);

    printf("Over Change\n");

    if(cur_team == 1){
        if(msg[10] == 1){
            // batting

            if(msg[9] == 1){
                msg[9] = 2;
            } else {
                msg[9] = 1;
            }

        } else {
            // balling
            int next_player = rand()%11;
            while(next_player == msg[14]){
                next_player = rand()%11;
            }
            msg[14] = next_player;
        }
        msg[6] = 1;
    } else {
        if(msg[10] == 1){
            // balling
            int next_player = rand()%11;
            while(next_player == msg[14]){
                next_player = rand()%11;
            }
            msg[14] = next_player;
        } else {
            // batting
            if(msg[9] == 1){
                msg[9] = 2;
            } else {
                msg[9] = 1;
            }
        }
        msg[7] == 1;
    }

    shmdt(msg);
}

void handle_innings_over(int sig){
    //
    int * msg = (int *)shmat(shmid, 0, 0);
    kill(msg[4], SIGKILL);
    kill(msg[5], SIGKILL);
    kill(msg[6], SIGKILL);
    shmdt(msg);
}

void process_team(int team){

    signal(SIGRTMIN+0, handle_next_over);
    signal(SIGRTMIN+1, handle_innings_over);

    cur_team = team;

    int fd_in;
    if(team == 1){
        fd_in = open("team1.txt", O_RDONLY); 
    } else if(team == 2){
        fd_in = open("team2.txt", O_RDONLY); 
    }
    if(fd_in<0) {
        perror("file cant be opened");
        return;
    }

    dup2(fd_in,0);  //(stdin :0)

    int seed_val;
    char * team_name = (char *) malloc(N*sizeof(char));
    team_players_seed = (int *) malloc(11*sizeof(int));
    team_players = (char **) malloc(N*sizeof(char *));

    scanf("%s", team_name);
    scanf("%d", &seed_val);

    for(int i = 0; i<11; i++){
        team_players[i] = (char *) malloc(N*sizeof(char));
        scanf("%s", team_players[i]);
    }

    for(int i = 0; i<11; i++){
        scanf("%d ", &team_players_seed[i]);
    }

    close(fd_in);


    int * msg = (int *)shmat(shmid, 0, 0);

    printf("Team %d: Team %s created:\n", team, team_name);
    // set ready
    msg[5+team] = 1;





    // for(int i = 0; i<11; i++){
    //     printf("Team %d: %s %d\n", team, team_players[i], team_players_seed[i]);
    // }


    // 1 = batting, 0 = bowling
    int task = -1;

    while(msg[8] == -1);

    if(msg[8] == 1){
        if(team == 1){
            task = 1;
        } else {
            task = 0;
        }
    } else {
        if(team == 1){
            task = 0;
        } else {
            task = 1;
        }
    }
    printf("Team %d: Toss recieved -> task %d\n", team, task);

    // player creation
    int pid_b1;
    int pid_b2;
    int pid_b;
    if(task == 1){
        // batting

        pid_b1 = fork();
        if(pid_b1 == 0){
            // batsman 1
            // printf("Team %d: batsman 1 fork()\n", team);
            process_batsman();
            exit(0);
        }
        // printf("fork b1: %d\n", pid_b1);
        pid_b2 = fork();
        if(pid_b2 == 0){
            // batsman 2
            // printf("Team %d: batsman 2 fork()\n", team);
            process_batsman();
            exit(0);
        }
        // printf("fork b2: %d\n", pid_b2);

        msg[4] = pid_b1;
        msg[5] = pid_b2;
    } else {
        // bowling

        pid_b = fork();
        if(pid_b == 0){
            // printf("Team %d: baller fork()\n", team);
            process_bowler();
            exit(0);
        }
        // printf("fork b: %d\n", pid_b);
        msg[3] = pid_b;
    }


    // printf("dddddddddddddddddd%d\n", msg[3]);

    sleep(1);

    printf("Team %d: Players ready:\n", team);

    // [0:umpire, 1:team_1, 2:team_2, 3:bowler, 4:batsman_1, 5:batsman_2, 6:team_1_ready, 7:team_2_ready, 8:umpire_toss, 9:curr_strike, 10:team_1_task, 11:baller_seed, 12:batsman_seed, 
    //  13:balls bowled in that innings, 14:current_bowler, 15:current_batsman_1, 16:current_batsman_2]
    printf("[0: umpire: %d, 1:team_1: %d, 2:team_2: %d, 3:bowler: %d, 4:batsman_1: %d, 5:batsman_2: %d, 6:team_1_ready: %d, 7:team_2_ready: %d, 8:umpire_toss: %d, 9:curr_strike: %d, 10:team_1_task: %d, 11:baller_seed: %d, 12:batsman_seed: %d, 13:balls bowled in that innings: %d, 14:current_bowler: %d, 15:current_batsman_1: %d, 16:current_batsman_2: %d]\n", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6], msg[7], msg[8], msg[9], msg[10], msg[11], msg[12], msg[13], msg[14], msg[15], msg[16]);
    // set ready
    msg[5+team] = 1;





    while(msg[17] == 0);

    shmdt(msg);
    exit(0);
}