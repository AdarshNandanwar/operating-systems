#include<stdio.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <string.h>
int main(int argc, char const *argv[])
{
	
	int fd_in = open("input.txt",O_RDONLY); 
	int fd_out = open("output.txt",O_WRONLY|O_TRUNC);


	if(fd_in<0 || fd_out<0)
	{
		perror("file cant be opened");
		return 0;
	}

	printf("fd_in = %d\nfd_out = %d\n",fd_in,fd_out);

	// int dup2(int oldfd, int newfd);
	dup2(fd_in,0);  //(stdin :0)
	dup2(fd_out,1);	//(stdout:1)

	int num;
	scanf("%d",&num);

	printf("phi(%d) = %d\n",num,num-1);

	close(fd_in);
	close(fd_out);
	return 0;
}