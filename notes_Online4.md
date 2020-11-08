# Templates
```c
#include<pthread.h>
#include<stdlib.h>
#include<asm/unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// #include <signal.h>
// #include <sys/time.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <time.h> 
// #include <sys/ipc.h>
// #include <sys/shm.h>
// #include <string.h>
```
## Parameter Structure
```c
typedef struct ParamStruct {
    int ** A, ** B, n, m, k;
} param_struct;
```
## Parameter passing and getting return value
```c
int main(int argc,char *argv[]){
    
    // CREATE
        
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr); 

    // struct param
    param_struct * param = (param_struct *) malloc(sizeof(param_struct));
    param->data = data;
    
    // int param
    int * param = (int *) malloc(sizeof(int));
    
	pthread_attr_setinheritsched(&attr,PTHREAD_INHERIT_SCHED); 	
	pthread_attr_getdetachstate(&attr,&val);
	if(val!= PTHREAD_CREATE_JOINABLE)
 		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    pthread_create(&tid, &attr, &runner, param);

    // JOIN

    // return int
    int * res_int;
    pthread_join(tid, (void **) &res_int);
    int value = *res_int;

    // return array
    int ** res_arr;
    pthread_join(tid, (void **) &res_arr);
    int value = res_arr[0][0];
}
```
## Runner Function
```c
void * runner (void * param) { 	
    param_struct * p = (param_struct *) param;

    int data = p->data;

    // return int
    int * res_int = (int *) malloc(sizeof(int));
	pthread_exit(res_int);

    // return array
    int ** res_arr = (int **) malloc(n*sizeof(int *));
    for(int i = 0; i<param_1->m; i++){
        res_arr[i] = (int *) malloc(param_1->n*sizeof(int));
    }
	pthread_exit(res_arr);
}
```

# C syntax notes

## Structures in C

```c
struct A {
    int value;
    char *label;
} ob1;

struct A ob2;
struct struct_a * p1 = (struct struct_a *) void_pointer
```
```c
typedef struct A {
    int value;
    char *label;
} struct_a;

struct_a ob1;
struct_a * p1 = (struct_a *) void_pointer
```