#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>

int main()
{
	//By default the exec command will replace the current process with the new process you pass on to it. Thus when need to do a task such as run a terminal command, would be better to fork and then use exec..!!
	int pid = fork();
	if(pid==0)
	{
		char *arg1[] = {0};
		char *arg2[] = {"ls","-l", 0};
		char *arg3[] = {"ping","www.google.com","-c","4", 0};
		char *arg4[] = {"which","chmod" ,0};

		char *env[] ={"PATH=/bin:/usr/bin","TERM=console",0};

		int output = execvp("ls",arg2);

		// int output = execl("/bin/pwd","pwd",0);
		// int output = execlp("ps","ps","-ax",0);
		// int output = execle("/usr/bin/which","which","which",0,env);
		// int output = execv("/usr/bin/which",arg4);
		// int output = execvp("ping", arg3);

		if(output<0)
		{
			printf("Command not found!\n");
			exit(-1);
		}
		exit(0);	
	}
	printf("From Main Function\n");
	exit(0);
}