#include<pthread.h>
#include<stdlib.h>
#include<asm/unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int * A, *B, n, m;

void * runner(void *param);

void reverse(int * arr, int n){
    for(int i = 0; i<n/2; i++){
        int temp = 0;
        temp = arr[i];
        arr[i] = arr[n-1-i];
        arr[n-1-i] = temp;
    }
}

int main(int argc,char *argv[])
{ 	
    int fd_in = open("input.txt", O_RDONLY); 
    if(fd_in<0) {
        perror("file cant be opened");
        return 0;
    }
    dup2(fd_in,0);  //(stdin :0)

    scanf("%d %d", &n, &m);

    A = (int *) malloc(n*sizeof(int));
    B = (int *) malloc(m*sizeof(int));
    for(int i = 0; i<n; i++) scanf("%d", &A[i]);
    for(int i = 0; i<m; i++) scanf("%d", &B[i]);

    reverse(A, n);
    reverse(B, m);

    pthread_t * tid_array = (pthread_t *) malloc((n+m)*sizeof(pthread_t));
    pthread_attr_t attr;
	pthread_attr_init(&attr); 

    for(int i = 0; i<n+m; i++){
        int * param = (int *) malloc(sizeof(int));
        param[0] = i;
        pthread_create(&tid_array[i], &attr, &runner, param);
    }
    int * result = (int *) malloc((n+m)*sizeof(int));
    for(int i = 0; i<n+m; i++){
        int * res;
        pthread_join(tid_array[i], (void **) &res);
        result[i] = res[0];
    }

    int carry = 0;
    for(int i = 0; i<n+m; i++){
        result[i] += carry;
        carry = result[i]/10;
        result[i] %= 10;
    }
    reverse(result, n+m);
    int zero = 1;
    for(int i = 0; i<n+m; i++){
        if(result[i] != 0) zero = 0;
        if(!zero) printf("%d", result[i]);
    }
    printf("\n");

    close(fd_in);
    return 0;
}

// runner multiplication thread function
void * runner (void * param)
{ 	
    int * place = param;
    int * res = (int *) malloc(sizeof(int));
    res[0] = 0;
    for(int i = 0; i<m; i++){
        if(0 <= place[0]-i && place[0]-i < n){
            res[0] += A[place[0]-i]*B[i];
        }
    }
    pthread_exit((void *) res);
}
