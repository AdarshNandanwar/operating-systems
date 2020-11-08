#include<pthread.h>
#include<stdlib.h>
#include<asm/unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct StructParamSub {
    int ** A, ** B, n, m, k;
} param_sub;

typedef struct StructParamMulti {
    param_sub * data;
    int r, c;
} param_multi;

void *runner_sub(void *param);
void *runner_multi(void *param);


int main(int argc,char *argv[])
{ 	
    int fd_in = open("input.txt", O_RDONLY); 
    if(fd_in<0) {
        perror("file cant be opened");
        return 0;
    }
    dup2(fd_in,0);  //(stdin :0)

    param_sub param;

    scanf("%d %d %d", &param.m, &param.k, &param.n);

    int ** A = (int **) malloc(param.m*sizeof(int *));
    for(int i = 0; i<param.m; i++){
        A[i] = (int *) malloc(param.k*sizeof(int));
        for(int j = 0; j<param.k; j++){
            scanf("%d", &A[i][j]);
        }
    }
    param.A = A;
    int ** B = (int **) malloc(param.k*sizeof(int *));
    for(int i = 0; i<param.k; i++){
        B[i] = (int *) malloc(param.n*sizeof(int));
        for(int j = 0; j<param.n; j++){
            scanf("%d", &B[i][j]);
        }
    }
    param.B = B;

    pthread_t tid_sub;
    pthread_attr_t attr;
	pthread_attr_init(&attr); 

    int ** result;
	pthread_create(&tid_sub, &attr, runner_sub, &param);
	pthread_join(tid_sub, (void **) &result);
    for(int i = 0; i<param.m; i++){
        for(int j = 0; j<param.n; j++){
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    close(fd_in);
    return 0;
}

// runner sub thread function
void * runner_sub (void * param)
{ 	
    param_sub * param_1 = (param_sub *) param;
    pthread_t * tid_array = (pthread_t *) malloc(param_1->m*param_1->n*sizeof(pthread_t));
    int ** result = (int **) malloc(param_1->m*param_1->n*sizeof(int *));
    for(int i = 0; i<param_1->m; i++){
        result[i] = (int *) malloc(param_1->n*sizeof(int));
    }

    for(int i = 0; i<param_1->m; i++){
        for(int j = 0; j<param_1->n; j++){
            param_multi * param_2 = (param_multi *)malloc(sizeof(param_multi));
            param_2->data = param_1;
            param_2->r = i;
            param_2->c = j;
            pthread_attr_t attr;
            pthread_attr_init(&attr); 
            pthread_create(&tid_array[i*param_1->n+j], &attr, &runner_multi, param_2);
        }
    }
    for(int i = 0; i<param_1->m; i++){
        for(int j = 0; j<param_1->n; j++){
            int * res;
	        pthread_join(tid_array[i*param_1->n+j], (void **) &res);
            result[i][j] = *res;
        }
    }
    
	pthread_exit(result);
}

// runner multiplication thread function
void * runner_multi (void * param)
{ 	
    param_multi * param_1 = (param_multi *) param;
    int r = param_1->r;
    int c = param_1->c;
    int ** A = param_1->data->A;
    int ** B = param_1->data->B;
    int k = param_1->data->k;

    int * res = (int *) malloc(sizeof(int));
    for(int i = 0; i<k; i++){
        res[0] += (A[r][i]*B[i][c]);
    }

	pthread_exit(res);
}
