#include<pthread.h>
#include<stdlib.h>
#include<asm/unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int * A, *B, *result, n, m;

void * runner_input (void * param){
    scanf("%d %d", &n, &m);
    A = (int *) malloc(n*sizeof(int));
    B = (int *) malloc(m*sizeof(int));
    result = (int *) malloc((n+m)*sizeof(int));
    for(int i = 0; i<n; i++) scanf("%d", &A[i]);
    for(int i = 0; i<m; i++) scanf("%d", &B[i]);
    pthread_exit(NULL);
}

int binary_search(int * arr, int sz, int x){
    int lo = 0, hi = sz-1, mid;
    while(lo<hi){
        mid = lo+(hi-lo)/2;
        if(arr[mid] > x){
            hi = mid;
        } else {
            lo = mid+1;
        }
    }
    if(arr[lo] > x){
        return lo;
    } else {
        return sz;
    }
}

void * runner_solve(void *param){
    int index = *(int *)param;
    int res;
    if(index<n){
        result[index+binary_search(B, m, A[index])] = A[index];
    } else {
        result[index-n+binary_search(A, n, B[index-n])] = B[index-n];
    }
    pthread_exit(NULL);
}

int main(int argc,char *argv[])
{ 	
    int fd_in = open("input.txt", O_RDONLY); 
    if(fd_in<0) {
        perror("file cant be opened");
        return 0;
    }
    dup2(fd_in,0);  //(stdin :0)


    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, &runner_input, NULL);
    pthread_join(tid, NULL);

    pthread_t * tid_array = (pthread_t *) malloc((n+m)*sizeof(pthread_t));
    for(int i = 0; i<n+m; i++){
        int * param = (int *) malloc(sizeof(int));
        *param = i;
        pthread_create(&tid_array[i], &attr, &runner_solve, param);
    }
    for(int i = 0; i<n+m; i++){
        pthread_join(tid_array[i], NULL);
    }
    
    for(int i = 0; i<n+m; i++){
        printf("%d ", result[i]);
    }
    printf("\n");

    close(fd_in);
    return 0;
}