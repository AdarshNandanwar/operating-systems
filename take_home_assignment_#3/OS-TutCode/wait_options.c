#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	for(int i = 0; i<10; i++)
	{
		pid_t x = fork();
		if(x == 0)
		{
			printf("Created Child PID: %d at index %d\n", getpid(), i);
			if(i >= 8)
				kill(getpid(), SIGSTOP);
			// sleep(1);
			// printf("Child PID: %d at index %d exiting\n", getpid(), i);
			exit(0);
		}
	}
	// wait(NULL);
	// for(int i = 0; i<10; i++)
	// {
	// 	printf("Loop CLASSIC: %d\n", waitpid(-1, NULL, 0));
	// }
	// sleep(2);
	// for(int i = 0; i<10; i++)
	// {
	// 	printf("Loop WNOHANG: %d\n", waitpid(-1, NULL, WNOHANG));
	// }
	for(int i = 0; i<15; i++)
	{
		pid_t x;
		printf("Loop WUNTRACED: %d\n", x=waitpid(-1, NULL, WUNTRACED));
		kill(x, SIGCONT);
	}
	printf("Main process done\n");
	return 0;
}