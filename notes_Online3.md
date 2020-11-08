# Templates

## Base

```c
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
#define N 1024
#define READ_END 0
#define WRITE_END 1

struct timeval start;
struct timeval end;
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}


int main(int argc, char * argv[]){

/*
    if(argc != 4) {
        printf("Please give parameter in the format: <arg1> <arg2> <arg3>\n");
        return -1;
    }
    int arg1 = atoi(argv[1]);
*/

    srand(time(NULL));
    gettimeofday(&start,0);

/*
    // PRINT TIME

    gettimeofday(&end,0);
    printf("%lf msec\n", timedifference_msec(start,end));
*/

    return 0;
}
```
## Print timestamp

```c
struct timeval start;
struct timeval end;
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

int main(int argc, char * argv[]){
    gettimeofday(&start,0);

    // PRINT TIME
    gettimeofday(&end,0);
    printf("%lf msec\n", timedifference_msec(start,end));
}
```

## Shared Memory
The three digits of the chmod code set permissions for these groups in this order:

1. Owner (you)
2. Group (a group of other users that you set up)
3. World (anyone else browsing around on the file system)

Each digit of this code sets permissions for one of these groups as follows. Read is 4. Write is 2. Execute is 1.

The sums of these numbers give combinations of these permissions:

- 0 = no permissions whatsoever; this person cannot read, write, or execute the file
- 1 = execute only
- 2 = write only
- 3 = write and execute (1+2)
- 4 = read only
- 5 = read and execute (4+1)
- 6 = read and write (4+2)
- 7 = read and write and execute (4+2+1)
```c
// SHARED MEMORY

// define memory in main
key_t key = ftok("/home/driver",68);
int shmid = shmget(IPC_PRIVATE, N*sizeof(int), 0666 | IPC_CREAT);

    // do this in every process
    int * msg = (int *)shmat(shmid, 0, 0);
    // do stuff
    shmdt(msg);

// at the end in main
shmctl(shmid, IPC_RMID, NULL);
```

## Signal
Common signals:
1. SIGSTOP - stops the process, can be collected by WUNTRACED
2. SIGCONT - continues the process
3. SIGTERM - gracefully kills the process
4. SIGKILL - kills the process immediately

```c
// definition of signal handler
void handle_signal_34(int sig){
    // do stuff
}

int process(){
    // assigning signal handler to a process
    signal(SIGRTMIN+0, handle_signal_34);

    // proper way to sleep for some time in a process accepting signals
    int time_remaining = time_interval;
    while(1){
        gettimeofday(&end,0);
        time_remaining = sleep(time_remaining);
        if(time_remaining == 0) break;
    } 

    // infinite sleep
    while(1);
}

// sending signal
kill(pid_target, SIGRTMIN+0);
// waits for other handler to finish according to priority (lower number has higher priority)
sigqueue(pid_target, SIGRTMIN+0, (const union sigval) NULL);
// signal to self
raise(SIGRTMIN+0);
```

### Recursive kill
```c
void handle_signal_34(int sig){
    int res;
    // loop through all child processes
    for(int i = 0; i<n; i++){
        kill(pid_child[i], SIGRTMIN+0);
        waitpid(pid_child[i], &status, 0);
        printf("status: %d\n", status/256);
    }
    exit(res);
}
```
## exec() family
```c
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
}
```

## Pipe 
### pwd function

```c
// function definition
void get_cwd(char * cwd){
    int pipe_cwd[2];
    if(pipe(pipe_cwd) == -1){
        printf("Couldn't create pipe for pwd.\n");
        return;
    }
    if(fork() == 0){
        dup2(pipe_cwd[WRITE_END], STDOUT_FILENO);
        close(pipe_cwd[READ_END]);
        close(pipe_cwd[WRITE_END]);
        execlp("pwd", "pwd", (char *)NULL);
    } else {
        waitpid(-1, NULL, 0);
        close(pipe_cwd[WRITE_END]);
        read(pipe_cwd[READ_END], cwd, N);
        close(pipe_cwd[READ_END]);
    }

    cwd[strlen(cwd)-1] = '\0';
}

// function call
char * cwd = (char *) malloc(N*sizeof(char));
get_cwd(cwd);
```

### Pipe General

```c
// PIPE

char * exec_output = (char *) malloc(N*sizeof(char));
int pipe_1[2];
if(pipe(pipe_1) == -1){
    printf("Couldn't create pipe.\n");
    return -1;
}
if(fork() == 0){
    dup2(pipe_1[WRITE_END], STDOUT_FILENO);
    close(pipe_1[READ_END]);
    close(pipe_1[WRITE_END]);
    execl("/bin/pwd", "pwd", (char *)NULL);
} else {
    waitpid(-1, NULL, 0);
    close(pipe_1[WRITE_END]);
    read(pipe_1[READ_END], exec_output, N);
    close(pipe_1[READ_END]);
}
// NOTE- exec_output contains '\n' in the end. To remove:
// exec_output[strlen(exec_output)-1] = '\0';
printf("%s", exec_output);
```

### Pipe Nested General
```c
// NESTED PIPE

char * exec_output = (char *) malloc(N*sizeof(char));
int pipe_1[2];
if(pipe(pipe_1) == -1){
    printf("Couldn't create pipe\n");
    return;
}
if(fork() == 0){
    // child
    dup2(pipe_1[WRITE_END], STDOUT_FILENO);
    close(pipe_1[READ_END]);
    close(pipe_1[WRITE_END]);
    execl("/bin/ls", "ls", "-F", (char *)NULL);
} else {
    // parent
    waitpid(-1, NULL, 0);
    close(pipe_1[WRITE_END]);

    // CHECKING USING grep
    int pipe_2[2];
    if(pipe(pipe_2) == -1){
        printf("Couldn't create pipe\n");
        return;
    }
    if(fork() == 0){
        // child
        dup2(pipe_1[READ_END], STDIN_FILENO);
        dup2(pipe_2[WRITE_END], STDOUT_FILENO);
        close(pipe_2[READ_END]);
        close(pipe_2[WRITE_END]);
        execl("/bin/grep", "grep", "/$", (char *)NULL);
    } else {
        // parent
        waitpid(-1, NULL, 0);
        close(pipe_2[WRITE_END]);
        read(pipe_2[READ_END], exec_output, N);
        close(pipe_2[READ_END]);
    }
    close(pipe_1[READ_END]);
}
// NOTE- exec_output contains '\n' in the end. To remove:
// exec_output[strlen(exec_output)-1] = '\0';
printf("%s", exec_output);
```

# File IO

## fgets()
- Haven't checked how this works
- Includes `\n` in the line read (except for the last line).
- If the last line is empty, it will read empty string and execute the loop.
- Use when you want to get some data from a file, NOT to simulate STDIN.
```c
FILE * fd_in = fopen("input.txt", "r");
if(fd_in == NULL){
    printf("error opening \"input.txt\" file\n");
    return 0;
}

char line[1024];
while(fgets(line, sizeof(line), fd_in)){
    printf("[%ld] %s", sizeof(line), line); // size if always 1024
    // do stuff
}

fclose(fd_in);
```

## dup2()
- This method can't detect EOF. Only use if number of input lines are known.
- Dosen't include `\n` in the line read. Behaves exactly like `STDIN`.
- Use when you want to simulate STDIN from a file.
- Options:  
    - O_RDONLY
    - O_WRONLY
    - O_WRONLY | O_TRUNC
```c
int fd_in = open("input.txt", O_RDONLY); 
if(fd_in<0) {
    perror("file cant be opened");
    return 0;
}

int fd_out = open("output.txt", O_WRONLY|O_TRUNC);
if(fd_out<0)
{
    perror("file cant be opened");
    return 0;
}

dup2(fd_in,0);  //(stdin :0)
dup2(fd_out,1);	//(stdout:1)

int n;
char line[1024];
scanf("%d", &n);
while(n--){
    scanf("%s", &line);
    printf("[%ld] %s", sizeof(line), line); // size if always 1024
    // do stuff
}

printf("Done\n");

close(fd_in);
close(fd_out);
```

# C syntax notes

## Scan complete line without space as delimiter
```c
scanf("%[^\n]%*c", line);
```
## To initalize a string
```c
#define N 1024
// DO:
char * str = (char *) malloc(N*sizeof(char));
strcat(str, "abcd");
// DON'T DO:
char * str = (char *) malloc(N*sizeof(char));
str = "abcd";
```
## flush STDOUT
```c
fflush(stdout);
```
## wait()
- value of status is `status/256`
- options:
    - WNOHANG
    - WUNTRACED
```c
int status;

wait(NULL);
wait(&status);
printf("status: %d\n", status/256);

waitpid(-1, NULL, 0);
waitpid(pid, &status, WNOHANG);
printf("status: %d\n", status/256);
```