#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> 

struct timeval start;
struct timeval end;
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

#define MAX_CAPACITY 20

int ticket_counter_size = 0, queue_size = 0, pid_ticket_counter = -1;

void helper_handler_queue_maintainer(int count_requested){
    int count_allowed = MAX_CAPACITY-queue_size < count_requested ? MAX_CAPACITY-queue_size : count_requested;
    gettimeofday(&end,0);
    printf("TICKET COUNTER:     %d people sent to ride queue\t\t- %lf msec\n", count_allowed, timedifference_msec(start,end));
    fflush(stdout);
    queue_size = queue_size+count_requested > MAX_CAPACITY ? MAX_CAPACITY : queue_size+count_requested;
    if(count_allowed > 0) {
        // printf("response handler, pid = %d, signal number = %d\n", pid_ticket_counter, SIGRTMIN+count_allowed-1);
        // printf("%d %d\n", pid_ticket_counter, SIGRTMIN+count_allowed-1);
        sigqueue(pid_ticket_counter, SIGRTMIN+count_allowed-1,(const union sigval)NULL);
    }
}

void handle_signal_34_ticket_counter(int sig){ticket_counter_size -= 1;}
void handle_signal_35_ticket_counter(int sig){ticket_counter_size -= 2;}
void handle_signal_36_ticket_counter(int sig){ticket_counter_size -= 3;}
void handle_signal_37_ticket_counter(int sig){ticket_counter_size -= 4;}
void handle_signal_38_ticket_counter(int sig){ticket_counter_size -= 5;}
void handle_signal_39_ticket_counter(int sig){ticket_counter_size -= 6;}
void handle_signal_40_ticket_counter(int sig){ticket_counter_size -= 7;}
void handle_signal_41_ticket_counter(int sig){ticket_counter_size -= 8;}
void handle_signal_42_ticket_counter(int sig){ticket_counter_size -= 9;}
void handle_signal_43_ticket_counter(int sig){ticket_counter_size -= 10;}
void handle_signal_44_ticket_counter(int sig){ticket_counter_size -= 11;}
void handle_signal_45_ticket_counter(int sig){ticket_counter_size -= 12;}
void handle_signal_46_ticket_counter(int sig){ticket_counter_size -= 13;}
void handle_signal_47_ticket_counter(int sig){ticket_counter_size -= 14;}
void handle_signal_48_ticket_counter(int sig){ticket_counter_size -= 15;}
void handle_signal_49_ticket_counter(int sig){ticket_counter_size -= 16;}
void handle_signal_50_ticket_counter(int sig){ticket_counter_size -= 17;}
void handle_signal_51_ticket_counter(int sig){ticket_counter_size -= 18;}
void handle_signal_52_ticket_counter(int sig){ticket_counter_size -= 19;}
void handle_signal_53_ticket_counter(int sig){ticket_counter_size -= 20;}


void handle_signal_34_queue_maintainer(int sig){helper_handler_queue_maintainer(1);}
void handle_signal_35_queue_maintainer(int sig){helper_handler_queue_maintainer(2);}
void handle_signal_36_queue_maintainer(int sig){helper_handler_queue_maintainer(3);}
void handle_signal_37_queue_maintainer(int sig){helper_handler_queue_maintainer(4);}
void handle_signal_38_queue_maintainer(int sig){helper_handler_queue_maintainer(5);}
void handle_signal_39_queue_maintainer(int sig){helper_handler_queue_maintainer(6);}
void handle_signal_40_queue_maintainer(int sig){helper_handler_queue_maintainer(7);}
void handle_signal_41_queue_maintainer(int sig){helper_handler_queue_maintainer(8);}
void handle_signal_42_queue_maintainer(int sig){helper_handler_queue_maintainer(9);}
void handle_signal_43_queue_maintainer(int sig){helper_handler_queue_maintainer(10);}
void handle_signal_44_queue_maintainer(int sig){helper_handler_queue_maintainer(11);}
void handle_signal_45_queue_maintainer(int sig){helper_handler_queue_maintainer(12);}
void handle_signal_46_queue_maintainer(int sig){helper_handler_queue_maintainer(13);}
void handle_signal_47_queue_maintainer(int sig){helper_handler_queue_maintainer(14);}
void handle_signal_48_queue_maintainer(int sig){helper_handler_queue_maintainer(15);}
void handle_signal_49_queue_maintainer(int sig){helper_handler_queue_maintainer(16);}
void handle_signal_50_queue_maintainer(int sig){helper_handler_queue_maintainer(17);}
void handle_signal_51_queue_maintainer(int sig){helper_handler_queue_maintainer(18);}
void handle_signal_52_queue_maintainer(int sig){helper_handler_queue_maintainer(19);}
void handle_signal_53_queue_maintainer(int sig){helper_handler_queue_maintainer(20);}

void handle_signal_54_queue_maintainer(int sig){gettimeofday(&end,0); printf("QUEUE MAINTAINER:   Queue Size before ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end)); queue_size = queue_size-1 < 0 ? 0 : queue_size-1; printf("QUEUE MAINTAINER:   Queue Size after ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end));}
void handle_signal_55_queue_maintainer(int sig){gettimeofday(&end,0); printf("QUEUE MAINTAINER:   Queue Size before ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end)); queue_size = queue_size-2 < 0 ? 0 : queue_size-2; printf("QUEUE MAINTAINER:   Queue Size after ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end));}
void handle_signal_56_queue_maintainer(int sig){gettimeofday(&end,0); printf("QUEUE MAINTAINER:   Queue Size before ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end)); queue_size = queue_size-4 < 0 ? 0 : queue_size-3; printf("QUEUE MAINTAINER:   Queue Size after ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end));}
void handle_signal_57_queue_maintainer(int sig){gettimeofday(&end,0); printf("QUEUE MAINTAINER:   Queue Size before ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end)); queue_size = queue_size-3 < 0 ? 0 : queue_size-4; printf("QUEUE MAINTAINER:   Queue Size after ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end));}
void handle_signal_58_queue_maintainer(int sig){gettimeofday(&end,0); printf("QUEUE MAINTAINER:   Queue Size before ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end)); queue_size = queue_size-5 < 0 ? 0 : queue_size-5; printf("QUEUE MAINTAINER:   Queue Size after ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end));}
void handle_signal_59_queue_maintainer(int sig){gettimeofday(&end,0); printf("QUEUE MAINTAINER:   Queue Size before ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end)); queue_size = queue_size-6 < 0 ? 0 : queue_size-6; printf("QUEUE MAINTAINER:   Queue Size after ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end));}
void handle_signal_60_queue_maintainer(int sig){gettimeofday(&end,0); printf("QUEUE MAINTAINER:   Queue Size before ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end)); queue_size = queue_size-7 < 0 ? 0 : queue_size-7; printf("QUEUE MAINTAINER:   Queue Size after ride: %d\t\t- %lf msec\n", queue_size, timedifference_msec(start,end));}


void process_ticket_counter(){

    int pid_queue_maintainer = getppid();

    signal(SIGRTMIN+0, handle_signal_34_ticket_counter);
    signal(SIGRTMIN+1, handle_signal_35_ticket_counter);
    signal(SIGRTMIN+2, handle_signal_36_ticket_counter);
    signal(SIGRTMIN+3, handle_signal_37_ticket_counter);
    signal(SIGRTMIN+4, handle_signal_38_ticket_counter);
    signal(SIGRTMIN+5, handle_signal_39_ticket_counter);
    signal(SIGRTMIN+6, handle_signal_40_ticket_counter);
    signal(SIGRTMIN+7, handle_signal_41_ticket_counter);
    signal(SIGRTMIN+8, handle_signal_42_ticket_counter);
    signal(SIGRTMIN+9, handle_signal_43_ticket_counter);
    signal(SIGRTMIN+10, handle_signal_44_ticket_counter);
    signal(SIGRTMIN+11, handle_signal_45_ticket_counter);
    signal(SIGRTMIN+12, handle_signal_46_ticket_counter);
    signal(SIGRTMIN+13, handle_signal_47_ticket_counter);
    signal(SIGRTMIN+14, handle_signal_48_ticket_counter);
    signal(SIGRTMIN+15, handle_signal_49_ticket_counter);
    signal(SIGRTMIN+16, handle_signal_50_ticket_counter);
    signal(SIGRTMIN+17, handle_signal_51_ticket_counter);
    signal(SIGRTMIN+18, handle_signal_52_ticket_counter);
    signal(SIGRTMIN+19, handle_signal_53_ticket_counter);

    int group_size, time_interval;
    // printf("%d, %d\n", pid_queue_maintainer, SIGRTMIN+19);
    // sigqueue(pid_queue_maintainer, SIGRTMIN+19,(const union sigval)NULL);

    // printf("Hi -%d!\n", getpid());
    // while(1);
    // printf("Bye! -%d\n", getpid());

    while(1){
        time_interval = 5+(rand()%16);
        // time_interval = 1+(rand()%3);
	    gettimeofday(&end,0);
        // printf("Ticket Counter process sleeping for %d sec at %lf msec\n", time_interval, timedifference_msec(start,end));
        
        // sleep(time_interval);
        int time_remaining = time_interval;
        while(1){
            gettimeofday(&end,0);
            // printf("Queue Maintainer sleeping for %d sec at %lf msec\n", time_remaining, timedifference_msec(start,end));
            time_remaining = sleep(time_remaining);
            if(time_remaining == 0) break;
        }    

        group_size = 1+(rand()%6);
        ticket_counter_size += group_size;

	    gettimeofday(&end,0);
        printf("TICKET COUNTER:     %d people bought a ticket\t\t- %lf msec\n", group_size, timedifference_msec(start,end));
        fflush(stdout);

        if(ticket_counter_size > 20){
            // printf("%d %d\n", pid_queue_maintainer, SIGRTMIN+19);
            sigqueue(pid_queue_maintainer, SIGRTMIN+19,(const union sigval)NULL);
        } else if(ticket_counter_size>0){
            // printf("%d %d\n", pid_queue_maintainer, SIGRTMIN+ticket_counter_size-1);
            sigqueue(pid_queue_maintainer, SIGRTMIN+ticket_counter_size-1,(const union sigval)NULL);
        }
    }
}

void process_queue_maintainer(int pid_driver){
    // printf("Enter -%d!\n", getpid());

    signal(SIGRTMIN+0, handle_signal_34_queue_maintainer);
    signal(SIGRTMIN+1, handle_signal_35_queue_maintainer);
    signal(SIGRTMIN+2, handle_signal_36_queue_maintainer);
    signal(SIGRTMIN+3, handle_signal_37_queue_maintainer);
    signal(SIGRTMIN+4, handle_signal_38_queue_maintainer);
    signal(SIGRTMIN+5, handle_signal_39_queue_maintainer);
    signal(SIGRTMIN+6, handle_signal_40_queue_maintainer);
    signal(SIGRTMIN+7, handle_signal_41_queue_maintainer);
    signal(SIGRTMIN+8, handle_signal_42_queue_maintainer);
    signal(SIGRTMIN+9, handle_signal_43_queue_maintainer);
    signal(SIGRTMIN+10, handle_signal_44_queue_maintainer);
    signal(SIGRTMIN+11, handle_signal_45_queue_maintainer);
    signal(SIGRTMIN+12, handle_signal_46_queue_maintainer);
    signal(SIGRTMIN+13, handle_signal_47_queue_maintainer);
    signal(SIGRTMIN+14, handle_signal_48_queue_maintainer);
    signal(SIGRTMIN+15, handle_signal_49_queue_maintainer);
    signal(SIGRTMIN+16, handle_signal_50_queue_maintainer);
    signal(SIGRTMIN+17, handle_signal_51_queue_maintainer);
    signal(SIGRTMIN+18, handle_signal_52_queue_maintainer);
    signal(SIGRTMIN+19, handle_signal_53_queue_maintainer);

    signal(SIGRTMIN+20, handle_signal_54_queue_maintainer);
    signal(SIGRTMIN+21, handle_signal_55_queue_maintainer);
    signal(SIGRTMIN+22, handle_signal_56_queue_maintainer);
    signal(SIGRTMIN+23, handle_signal_57_queue_maintainer);
    signal(SIGRTMIN+24, handle_signal_58_queue_maintainer);
    signal(SIGRTMIN+25, handle_signal_59_queue_maintainer);
    signal(SIGRTMIN+26, handle_signal_60_queue_maintainer);

    int time_remaining = 100;

    // sleep(time_remaining);
    while(1){
        gettimeofday(&end,0);
        // printf("Queue Maintainer sleeping for %d sec at %lf msec\n", time_remaining, timedifference_msec(start,end));
        time_remaining = sleep(time_remaining);
        if(time_remaining == 0) break;
    }

    gettimeofday(&end,0);
    printf("QUEUE MAINTAINER:   closing\t\t\t\t- %lf msec\n", timedifference_msec(start,end));
    kill(pid_ticket_counter, SIGTERM);
    kill(pid_driver, SIGTERM);
    exit(0);
    // Another method is to wait in main and send a signal here to kill the child processes.
    // In this way, not sleep is interrupted and process ends in exactly 100 secs and main dosen't need to know the pid of every grandchildren.
}

void process_driver(){
    int pid_queue_maintainer = getppid();
    int empty_seats, time_interval;

    while(1){
        time_interval = 10+(rand()%6);
        // time_interval = 1+(rand()%3);
	    gettimeofday(&end,0);
        // printf("Driver process sleeping for %d sec at %lf msec\n", time_interval, timedifference_msec(start,end));
        sleep(time_interval);
        empty_seats = 1+(rand()%7);
	    gettimeofday(&end,0);
        printf("DRIVER:             Ride Capacity: %d\t\t\t- %lf msec\n", empty_seats, timedifference_msec(start,end));
        fflush(stdout);
        // printf("%d %d\n", pid_queue_maintainer, SIGRTMIN+19+empty_seats);
        sigqueue(pid_queue_maintainer, SIGRTMIN+19+empty_seats,(const union sigval)NULL);
    }
}

int main(int argc, char *argv[])
{
	gettimeofday(&start,0);
    int pid_queue_maintainer = -1, pid_driver = -1;
    srand(time(0));

    // PROCESS TREE:
    //                     main
    //                     /
    //         queue_maintainer
    //             /       \
    // ticket_counter      driver

    
    pid_queue_maintainer = fork();
    if(pid_queue_maintainer == 0){
        printf("pid of queue maintainer: %d\n", getpid());
        pid_ticket_counter = fork();
        if(pid_ticket_counter == 0){
            process_ticket_counter();
            exit(0);
        }
        printf("pid of ticket counter: %d\n", pid_ticket_counter);
        pid_driver = fork();
        if(pid_driver == 0){
            process_driver();
            exit(0);
        }
        printf("pid of driver: %d\n", pid_driver);
        process_queue_maintainer(pid_driver);
    }

    while(wait(NULL) != -1);

	return 0;
}