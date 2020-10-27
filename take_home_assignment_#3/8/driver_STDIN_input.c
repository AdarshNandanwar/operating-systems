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
#define N 2048
#define READ_END 0
#define WRITE_END 1

struct timeval start;
struct timeval end;
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

void exec_external_command(char cmd[], int pipe_prev[2], int logging){

    char * cur_cmd = (char *) malloc(N*sizeof(char));
    char * next_cmd = (char *) malloc(N*sizeof(char));
    int i = 0, j = 0, k = 0, is_last_cmd = 1, is_first_cmd = 0;

    while(i<strlen(cmd) && cmd[i] != '|'){
        cur_cmd[j++] = cmd[i++];
    }
    cur_cmd[j] = '\0';
    if(i<strlen(cmd)){
        is_last_cmd = 0;
        j = 0;
        i++;
        while(i<strlen(cmd)){
            next_cmd[j++] = cmd[i++];
        }
        next_cmd[j] = '\0';
    }
    int word_count = 0, is_prev_space = 1;
    i = 0;
    while(i<strlen(cur_cmd)){
        if(cur_cmd[i] != ' '){
            if(is_prev_space){
                word_count++;
            }
            is_prev_space = 0;
            i++;
        } else {
            is_prev_space = 1;
            while(i<strlen(cur_cmd) && cur_cmd[i] == ' '){
                i++;
            }
        }
    }
    char ** arg = (char **) malloc((word_count+1)*sizeof(char *));
    i = 0, j = 0; 
    for(i = 0; i<word_count; i++){
        arg[i] = (char *) malloc(N*sizeof(char));
        k = 0;
        while(j<strlen(cur_cmd) && cur_cmd[j] == ' '){
            j++;
        }
        while(j<strlen(cur_cmd) && cur_cmd[j] != ' '){
            arg[i][k++] = cur_cmd[j++];
        }
        arg[i][k] = '\0';
    }
    arg[word_count] = 0;

    if(pipe_prev[0] == -1){
        is_first_cmd = 1;
    }


    // checking if command is available in environment PATH
    int pipe_check[2];
    char * which_output = (char *) malloc(N*sizeof(char));
    if(pipe(pipe_check) == -1){
        printf("Couldn't create pipe.\n");
        return;
    }
    if(fork() == 0){
        dup2(pipe_check[WRITE_END], STDOUT_FILENO);
        close(pipe_check[READ_END]);
        close(pipe_check[WRITE_END]);
        execlp("which", "which", arg[0], (char *)NULL);
    } else {
        waitpid(-1, NULL, 0);
        close(pipe_check[WRITE_END]);
        read(pipe_check[READ_END], which_output, N);
        close(pipe_check[READ_END]);
    }
    if(strlen(which_output) != 0 && which_output[strlen(which_output)-1] == '\n'){
        which_output[strlen(which_output)-1] = '\0';
    }


    if(strlen(which_output) == 0){
        printf("command not found in environment variable PATH\n");
        printf("searching in cwd: ");

        // getting cwd
        int pipe_cwd[2];
        char * cwd = (char *) malloc(N*sizeof(char));
        if(pipe(pipe_cwd) == -1){
            printf("Couldn't create pipe.\n");
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
        printf("%s\n", cwd);

        // checking if command is available in cwd
        int pipe_check_cwd[2];
        char * which_output_cwd = (char *) malloc(N*sizeof(char));
        char ** env = (char **) malloc(2*sizeof(char *));
        env[0] = (char *) malloc(N*sizeof(char));
        env[1] = (char *) malloc(N*sizeof(char));
        strcat(env[0], "PATH=");
        strcat(env[0], cwd);
        env[1] = 0;


        if(pipe(pipe_check_cwd) == -1){
            printf("Couldn't create pipe.\n");
            return;
        }
        if(fork() == 0){
            dup2(pipe_check_cwd[WRITE_END], STDOUT_FILENO);
            close(pipe_check_cwd[READ_END]);
            close(pipe_check_cwd[WRITE_END]);
            execle("/usr/bin/which", "which", arg[0], (char *)NULL, env);
        } else {
            waitpid(-1, NULL, 0);
            close(pipe_check_cwd[WRITE_END]);
            read(pipe_check_cwd[READ_END], which_output_cwd, 1000);
            close(pipe_check_cwd[READ_END]);
        }
        
        if(strlen(which_output_cwd) == 0){
            printf("command not found in cwd: %s\nThe command %s not found\n", cwd, arg[0]);
            return;
        } else {
            printf("command found in cwd\n");
            execve(arg[0], arg, env);
            return;
        }     
    } else {
        printf("command found in env PATH\n");

        // command found in environment variable PATH
        int pipe_next[2];
        if(pipe(pipe_next) == -1){
            printf("Couldn't create pipe\n");
            return;
        }

        if(fork() == 0){
            if(is_first_cmd == 0){
                dup2(pipe_prev[READ_END], STDIN_FILENO);
                close(pipe_prev[READ_END]);
            }
            dup2(pipe_next[WRITE_END], STDOUT_FILENO);
            close(pipe_next[READ_END]);
            close(pipe_next[WRITE_END]);
            execvp(arg[0], arg);
        } else {
            // parent
            waitpid(-1, NULL, 0);
            close(pipe_next[WRITE_END]);
            if(is_first_cmd == 0){
                close(pipe_prev[READ_END]);
            }
            if(is_last_cmd){
                char * exec_output = (char *) malloc(N*sizeof(char));
                read(pipe_next[READ_END], exec_output, N);
                printf("%s", exec_output);
                fflush(stdout);
                if(logging){
                    FILE * fd_out_log = fopen("output.log", "a");
                    if(fd_out_log == NULL){
                        printf("error opening \"output.log\" file\n");
                        return;
                    }
                    fprintf(fd_out_log, "%s\n", exec_output);
                    fclose(fd_out_log);
                }
            } else {
                exec_external_command(next_cmd, pipe_next, logging);
            }
            close(pipe_next[READ_END]);
        }
    }
}

int main(int argc, char * argv[]){
    
    srand(time(NULL));
    gettimeofday(&start,0);

    fclose(fopen("output.log", "w"));
    fclose(fopen("command.log", "w"));

    // simulate
    
    char * line = (char *) malloc(N*sizeof(char));
    int entered = 0, logging = 0;

    while(1){
        printf("> ");
        scanf("%[^\n]%*c", line);
        if(entered == 0){
            if(strcmp(line, "entry") == 0){
                entered = 1;
                printf("Command line interpreter started\n");
            } else {
                printf("Command line interpreter not started\n");
            }
        } else {
            if(strcmp(line, "entry") == 0){
                printf("Command line interpreter already running\n");
            } else if(strcmp(line, "exit") == 0){
                entered = 0;
                logging = 0;
                printf("Logging stopped\n");
                printf("Command line interpreter exited\n");
            } else if(strcmp(line, "log") == 0){
                if(logging){
                    printf("Logging already running\n");
                } else {
                    logging = 1;
                    printf("Logging started\n");
                }
            } else if(strcmp(line, "unlog") == 0){
                if(logging){
                    logging = 0;
                    printf("Logging stopped\n");
                } else {
                    printf("Logging already stopped\n");
                }
            } else if(strcmp(line, "viewcmdlog") == 0){
                FILE * fd_cmd_log = fopen("command.log", "r");
                if(fd_cmd_log == NULL){
                    printf("error opening \"command.log\" file\n");
                    return 0;
                }
                char * log_line = (char *) malloc(1024*sizeof(char));
                while(fgets(log_line, sizeof(log_line), fd_cmd_log)){
                    printf("%s", log_line);
                }
                fclose(fd_cmd_log);
            } else if(strcmp(line, "viewoutlog") == 0){
                FILE * fd_out_log = fopen("output.log", "r");
                if(fd_out_log == NULL){
                    printf("error opening \"output.log\" file\n");
                    return 0;
                }
                char * out_line = (char *) malloc(1024*sizeof(char));
                while(fgets(out_line, sizeof(out_line), fd_out_log)){
                    printf("%s", out_line);
                }
                fclose(fd_out_log);
            } else {
                if(logging){
                    FILE * fd_cmd_log = fopen("command.log", "a");
                    if(fd_cmd_log == NULL){
                        printf("error opening \"command.log\" file\n");
                        return 0;
                    }
                    fprintf(fd_cmd_log, "%s\n", line);
                    fclose(fd_cmd_log);
                }
                if(strlen(line) >= 10){
                    int is_change_dir = 1;
                    char * prefix = (char *) malloc(N*sizeof(char));
                    strcat(prefix, "changedir ");
                    for(int i = 0; i<10; i++){
                        if(line[i] != prefix[i]){
                            is_change_dir = 0;
                        }
                    }
                    if(is_change_dir){
                        char * path = (char *) malloc(N*sizeof(char));
                        int index = 0;
                        for(int i = 10; i<strlen(line); i++){
                            path[index++] = line[i];
                        }
                        path[index] = '\0';
                        if(chdir(path) < 0) {
                            printf("Error changing directory to %s\n", path);
                            return 0;
                        }
                        printf("Directory changed. The present working directory is ");
                        int pipe_pwd[2];
                        if(pipe(pipe_pwd) == -1){
                            printf("Couldn't create pipe_pwd\n");
                            return 0;
                        }

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
                        }
                        else {
                            waitpid(-1, NULL, 0);
                            close(pipe_1[WRITE_END]);
                            char * exec_output = (char *) malloc(N*sizeof(char));
                            read(pipe_1[READ_END], exec_output, N);
                            close(pipe_1[READ_END]);
                            printf("%s", exec_output);
                        }
                        continue;
                    }
                }

                // EXTERNAL COMMAND
                int pipe_next[2] = {-1, -1};
                exec_external_command(line, pipe_next, logging);
            }
        }
    }

    return 0;
}