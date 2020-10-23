#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main()
{
	int p[2];
	if(pipe(p) == -1)
		printf("Couldn't create pipe\n");
	printf("STDIN: %d\n", STDIN_FILENO);
	printf("STDOUT: %d\n", STDOUT_FILENO);
	printf("p[READ_END]: %d\n", p[READ_END]);
	printf("p[WRITE_END]: %d\n", p[WRITE_END]);
	int x = fork();
	char s[1000];
	if(x == 0)
	{
		dup2(p[WRITE_END], STDOUT_FILENO);
		close(p[READ_END]);
		close(p[WRITE_END]);
		// close(STDOUT_FILENO);
		execl("/bin/ls", "ls", "-l", (char *)NULL);
	}
	else
	{
		waitpid(-1, NULL, 0);
		x = fork();
		if(x == 0)
		{
			// int n = 0;
			// char *c;
			close(p[WRITE_END]);
			// while(read(p[READ_END], c, 1)!=0)
			// {
			// 	printf("%c", c[0]);
			// 	n++;
			// }
			// printf("%d\n", n);

			// read(p[READ_END], s, 1000);
			// printf("%s\n", s);
			
			dup2(p[READ_END], STDIN_FILENO);
			// close(p[READ_END]);
			// execl("/bin/grep", "grep", "demo", (char *)NULL);
			execl("/usr/bin/wc", "wc", "-c", (char *)NULL);
		}
		close(p[WRITE_END]);
		close(p[READ_END]);
		waitpid(-1, NULL, 0);
	}
	return 0;
}