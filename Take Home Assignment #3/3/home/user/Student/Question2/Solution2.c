#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define N 16
#define READ_END 0
#define WRITE_END 1

void find_target_loop(char * path, int shmid, char * target){

    if(chdir(path) < 0) {
        printf("Error changing directory to %s\n", path);
        return;
    }

    char *msg = (char *)shmat(shmid, 0, 0);

    int pipe_1_end[2];
    if(pipe(pipe_1_end) == -1){
        printf("Couldn't create pipe at path: %s\n", path);
        return;
    }

    if(fork() == 0){
        // child
        dup2(pipe_1_end[WRITE_END], STDOUT_FILENO);
        close(pipe_1_end[READ_END]);
        close(pipe_1_end[WRITE_END]);
        execl("/bin/ls", "ls", "-F", (char *)NULL);
    } else {
        // parent
        waitpid(-1, NULL, 0);
        close(pipe_1_end[WRITE_END]);

        // MANUAL CHECKING

        char file_list[1000];
        char file_name[260];
        char * next_path = NULL;

        read(pipe_1_end[READ_END], file_list, 1000);

        int j = 0;
        for(int i = 0; i<strlen(file_list); i++){
            if(strcmp(msg, "0")) break;
            if(file_list[i] == '\n'){
                file_name[j] = '\0';
                if(strlen(file_name)-1 >= 0 && file_name[strlen(file_name)-1] == '/'){
                    // if folder
                    next_path = malloc(strlen(path)+strlen(file_name)+1);
                    strcat(next_path, path);
                    strcat(next_path, file_name);
                    if(!fork()){
                        find_target_loop(next_path, shmid, target);
                        exit(0);
                    }
                } else {
                    // check the file
                    if(strcmp(file_name, target) == 0){
                        if(strcmp(msg, "0") == 0){
                            strcpy(msg, path);
                        }
                    }
                }
                j = 0;
            } else {
                file_name[j] = file_list[i]; 
                j++;
            }
        }
        
        close(pipe_1_end[READ_END]);
    }

    while(wait(NULL) > 0);
}

void find_target_grep(char * path, int shmid, char * target){

    if(chdir(path) < 0) {
        printf("Error changing directory to %s\n", path);
        return;
    }


    char *msg = (char *)shmat(shmid, 0, 0);

    // checking if file in cwd

    int pipe_1_end[2];
    if(pipe(pipe_1_end) == -1){
        printf("Couldn't create pipe at path: %s\n", path);
        return;
    }

    if(fork() == 0){
        // child
        dup2(pipe_1_end[WRITE_END], STDOUT_FILENO);
        close(pipe_1_end[READ_END]);
        close(pipe_1_end[WRITE_END]);
        execl("/bin/ls", "ls", "-F", (char *)NULL);
    } else {
        // parent
        waitpid(-1, NULL, 0);
        close(pipe_1_end[WRITE_END]);

        // CHECKING USING grep
        int pipe_2_end[2];
        if(pipe(pipe_2_end) == -1){
            printf("Couldn't create pipe at path: %s\n", path);
            return;
        }

        if(fork() == 0){
            // child
            dup2(pipe_1_end[READ_END], STDIN_FILENO);
            dup2(pipe_2_end[WRITE_END], STDOUT_FILENO);
            close(pipe_2_end[READ_END]);
            close(pipe_2_end[WRITE_END]);
            execl("/bin/grep", "grep", "-wc", target, (char *)NULL);
        } else {
            // parent
            waitpid(-1, NULL, 0);
            close(pipe_2_end[WRITE_END]);

            char word_count[1];

            read(pipe_2_end[READ_END], word_count, 1);
            close(pipe_2_end[READ_END]);

            if(word_count[0] != '0'){
                if(strcmp(msg, "0")){
                    // file found
                    return;
                } else {
                    printf("%s\n", target);
                    strcpy(msg, path);   
                }
            }
        }
        close(pipe_1_end[READ_END]);
    }
    while(wait(NULL) > 0);



    // file not in cwd. searching in subdirectories
    //checking shared memory if the file is found
    if(strcmp(msg, "0")){
        // file found
        return;
    }



    int pipe_3_end[2];
    if(pipe(pipe_3_end) == -1){
        printf("Couldn't create pipe at path: %s\n", path);
        return;
    }

    if(fork() == 0){
        // child
        dup2(pipe_3_end[WRITE_END], STDOUT_FILENO);
        close(pipe_3_end[READ_END]);
        close(pipe_3_end[WRITE_END]);
        execl("/bin/ls", "ls", "-F", (char *)NULL);
    } else {
        // parent
        waitpid(-1, NULL, 0);
        close(pipe_3_end[WRITE_END]);

        // CHECKING USING grep
        int pipe_4_end[2];
        if(pipe(pipe_4_end) == -1){
            printf("Couldn't create pipe at path: %s\n", path);
            return;
        }

        if(fork() == 0){
            // child
            dup2(pipe_3_end[READ_END], STDIN_FILENO);
            dup2(pipe_4_end[WRITE_END], STDOUT_FILENO);
            close(pipe_4_end[READ_END]);
            close(pipe_4_end[WRITE_END]);
            execl("/bin/grep", "grep", "/$", (char *)NULL);
        } else {
            // parent
            waitpid(-1, NULL, 0);
            close(pipe_4_end[WRITE_END]);

            char dir_list[1000];

            read(pipe_4_end[READ_END], dir_list, 1000);
            close(pipe_4_end[READ_END]);

            char dir_name[260];
            char * next_path = NULL;
            int j = 0;
            for(int i = 0; i<strlen(dir_list); i++){
                if(dir_list[i] == '\n'){
                    dir_name[j] = '\0';
                    next_path = malloc(strlen(path)+strlen(dir_name)+1);
                    strcat(next_path, path);
                    strcat(next_path, dir_name);
                    if(!fork()){
                        find_target_grep(next_path, shmid, target);
                        exit(0);
                    }
                    j = 0;
                } else {
                    dir_name[j] = dir_list[i]; 
                    j++;
                }
            }
        }
        close(pipe_3_end[READ_END]);
    }
    while(wait(NULL) > 0);
}

int main(int argc, char * argv[]){

    if(argc != 4) {
        printf("Please give parameter in the format: <filename> <ID No.> <New marks>\n");
        return -1;
    }

    // printf("file name: %s\nid no.: %s\nnew marks: %s\n", argv[1], argv[2], argv[3]);


    // CREATING SHARED MEMORY
    key_t key = ftok("/Solution2.c", 68);
    int shmid = shmget(IPC_PRIVATE, N*sizeof(char), 0666 | IPC_CREAT);
    char *msg = (char *)shmat(shmid, 0, 0);
    strcpy(msg, "0");

    char * s, * target = malloc(strlen(argv[1]));
    strcpy(target, argv[1]);

    // creating pipe for getting the output of exec
    int pipe_end[2];
    if(pipe(pipe_end) == -1){
        printf("Couldn't create pipe.\n");
        return -1;
    }

    int pid1 = fork();
    if(pid1 == 0){
        // child
        dup2(pipe_end[WRITE_END], STDOUT_FILENO);
        close(pipe_end[READ_END]);
        close(pipe_end[WRITE_END]);
        execl("/bin/pwd", "pwd", (char *)NULL);
    }
    else {
        // parent
        waitpid(-1, NULL, 0);
        close(pipe_end[WRITE_END]);

        char path[1000];
        read(pipe_end[READ_END], path, 1000);
        close(pipe_end[READ_END]);
        path[strlen(path)-2] = 0;
        strcat(path, "/../../Evaluator/");
        
        
        strcat(target, "*");
        find_target_loop(path, shmid, target);
        
        // strcat(target, "\\*");
        // find_target_grep(path, shmid, target);
    }

    while(wait(NULL)!=-1);

    if(strcmp(msg, "0") == 0) {
        printf("File not found\n");
        return 0;
    }

    // EDITING MARKS
    // char *file_path = malloc(strlen(msg)+strlen(target));
    char file_path[1000], temp_file_path[1000];
    strcpy(file_path, msg);
    strcpy(temp_file_path, msg);
    strcat(file_path, argv[1]);
    strcat(temp_file_path, "temp_file.txt");

    printf("%s\n", file_path);

    FILE * f1 = fopen(file_path, "r");
    FILE * f2 = fopen(temp_file_path, "w");

    printf("%s\n", file_path);
    printf("%s\n", temp_file_path);

    if(f1 == NULL || f2 == NULL){
        printf("error opening file\n");
        return 0;
    }

    int i, j;
    char id[14], line[100];
    id[13] = '\0';
    while(fgets(line, sizeof(line), f1)){
        for(i = 0; i<13; i++){
            id[i] = line[i];
        }
        if(strcmp(id, argv[2]) == 0){
            fprintf(f2, "%s\t%s\n", argv[2], argv[3]);
        } else {
            fprintf(f2, "%s", line);
        }
    }

    fclose(f1);
    fclose(f2);

    if(fork() == 0){
        execl("/bin/mv", "mv", temp_file_path, file_path, (char *)NULL);
    } else {
        waitpid(-1, NULL, 0);
    }

    return 0;
}