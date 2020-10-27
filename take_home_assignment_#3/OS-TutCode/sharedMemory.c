#include<stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define N 16
int main(int argc, char const *argv[])
{
	// key_t ftok(const char *pathname, int proj_id);
	key_t key = ftok("/home/OS_tutorial",68);


	// int shmget(key_t key, size_t size, int shmflg);
	int shmid = shmget(IPC_PRIVATE, N*sizeof(char), 0666 | IPC_CREAT);

	if(fork() == 0)
	{
		/*CHILD PROCESS*/


		// void *shmat(int shmid, const void *shmaddr, int shmflg);  
		// ( shmflg: SHM_RDONLY, SHM_EXEC)
		char *msg = (char *)shmat(shmid, 0, 0);
		
		sleep(2); 
		printf("Message received in child process: %s\n",msg );
		strcpy(msg,"Hello parent");
		

		// int shmdt(const void *shmaddr);
		shmdt(msg);
		exit(0);

	}
	else
	{
		/*PARENT PROCESS*/


		char *msg = (char *)shmat(shmid,(void *)0,0);
		
		strcpy(msg,"Hi Child !!");
		sleep(4);
		printf("Message received in parent process: %s\n",msg );


		wait(NULL);

		// int shmdt(const void *shmaddr);
		shmdt(msg);
	}

	// int shmctl(int shmid, int cmd, struct shmid_ds *buf);
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}