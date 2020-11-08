#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "convNet.h"


void getData(FILE *inputFile, ConvNetInput *convNetInput)
{
    // Image input.
    fscanf(inputFile, "%d", &convNetInput->convImageDim);
    convNetInput->convImage = malloc(sizeof(int *) * convNetInput->convImageDim);
    for (int i = 0; i < convNetInput->convImageDim; i++)
    {
        (convNetInput->convImage)[i] = (int *) malloc(sizeof(int) * convNetInput->convImageDim);
        for (int j = 0; j < convNetInput->convImageDim; j++)
        {
            fscanf(inputFile, " %d", &convNetInput->convImage[i][j]);
        }
    }

    // Kernel Input.
    fscanf(inputFile, "%d", &convNetInput->convKernelDim);
    convNetInput->convKernel = malloc(sizeof(int *) * convNetInput->convKernelDim);
    for (int i = 0; i < convNetInput->convKernelDim; i++)
    {
        convNetInput->convKernel[i] = (int *) malloc(sizeof(int) * convNetInput->convKernelDim);
        for (int j = 0; j < convNetInput->convKernelDim; j++)
        {
            fscanf(inputFile, " %d", &convNetInput->convKernel[i][j]);
        }
    }

    fscanf(inputFile, "%d", &convNetInput->convStride);

    // Calculate the number of weights required for the fully-connected layer.
    int sqrRootWeightsNum = ((convNetInput->convImageDim) - (convNetInput->convKernelDim)) / (convNetInput->convStride) + 1;
    convNetInput->fullyConnWeightsNum = sqrRootWeightsNum * sqrRootWeightsNum;
    convNetInput->fullyConnWeights = (int *) malloc(sizeof(int) * convNetInput->fullyConnWeightsNum);
    for (int i = 0; i < convNetInput->fullyConnWeightsNum; i++)
    {
        fscanf(inputFile, " %d", &(convNetInput->fullyConnWeights)[i]);
    }

    return;
}

int convNet(char *fileName){

    // GETTING INPUT

    ConvNetInput * cni = (ConvNetInput *) malloc(sizeof(ConvNetInput));
    FILE * fin = fopen(fileName, "r");
    if(fin == NULL){
        printf("ERROR [file could not be opened]\n");
        return 0;
    }

    getData(fin, cni);
    fclose(fin);


    // DETACHABLE THREAD: PRINT INPUT

    pthread_t tid_print;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int val;
	pthread_attr_getdetachstate(&attr,&val);
	if(val == PTHREAD_CREATE_JOINABLE)
 		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&tid_print, &attr, runner_print_input, cni);


    // JOINABLE THREADS: CL

    int clDim =  ((cni->convImageDim) - (cni->convKernelDim)) / (cni->convStride) + 1;
    int ** CLOut = (int **) malloc(clDim* sizeof(int *));

    pthread_t ** tid_CL = (pthread_t **) malloc(clDim*sizeof(pthread_t *));
    for(int i = 0; i<clDim; i++){
        tid_CL[i] = (pthread_t *) malloc(clDim*sizeof(pthread_t));
        CLOut[i] = (int *) malloc(clDim*sizeof(int));
    }

    for(int i = 0; i<clDim; i++){
        for(int j = 0; j<clDim; j++){
            cl_param * param_cl = (cl_param *) malloc(sizeof(cl_param));
            param_cl->cni = cni;
            param_cl->row = cni->convStride*i;
            param_cl->col = cni->convStride*j;

            pthread_attr_init(&attr);
            int state_val;
            pthread_attr_getdetachstate(&attr,&state_val);
            if(state_val != PTHREAD_CREATE_JOINABLE)
                pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
            pthread_create(&tid_CL[i][j], &attr, runner_CL_layer, param_cl);
        }
    }

    for(int i = 0; i<clDim; i++){
        for(int j = 0; j<clDim; j++){
            int * res = (int *) malloc(sizeof(int));
            pthread_join(tid_CL[i][j], (void **) &res);
            CLOut[i][j] = *res;
        }
    }
    

    // JOINABLE THREAD: PRINT CLOut

    cl_print_param * param_print_cl = (cl_print_param *) malloc(sizeof(cl_print_param));
    param_print_cl->CLOut = CLOut;
    param_print_cl->n = clDim;
    pthread_attr_init(&attr);
	pthread_attr_getdetachstate(&attr,&val);
	if(val != PTHREAD_CREATE_JOINABLE)
 		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    pthread_create(&tid_print, &attr, runner_print_CL, param_print_cl);




    pthread_join(tid_print, NULL);


    // JOINABLE THREADS: FILL OneDOut

    int * OneDOut = (int *) malloc(clDim*clDim*sizeof(int));

    pthread_t * tid_1d = (pthread_t *) malloc(clDim*sizeof(pthread_t));
    for(int i = 0; i<clDim; i++){
        cl_1d_param * param_1d_cl = (cl_1d_param *) malloc(sizeof(cl_1d_param));
        param_1d_cl->CLOut = CLOut;
        param_1d_cl->OneDOut = OneDOut;
        param_1d_cl->n = clDim;
        param_1d_cl->row = i;

        pthread_attr_init(&attr);
        int state_val;
        pthread_attr_getdetachstate(&attr,&state_val);
        if(state_val != PTHREAD_CREATE_JOINABLE)
            pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
        pthread_create(&tid_1d[i], &attr, runner_1d, param_1d_cl);
    }
    for(int i = 0; i<clDim; i++){
        pthread_join(tid_1d[i], NULL);
    }

    // THREAD: GENERATE Y

    pthread_t tid_y;
    y_param * param_y = (y_param *) malloc(sizeof(y_param));
    param_y->weights = cni->fullyConnWeights;
    param_y->OneDOut = OneDOut;
    param_y->n = clDim*clDim;
    pthread_attr_init(&attr);
	pthread_attr_getdetachstate(&attr,&val);
	if(val != PTHREAD_CREATE_JOINABLE)
 		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    pthread_create(&tid_y, &attr, runner_y, param_y);

    int * Y = (int *) malloc(sizeof(int));
    pthread_join(tid_y, (void **) &Y);
    
    
    return *Y;
}


// THREAD RUNNER FUNCTIONS

void * runner_print_input(void * param){
    
    ConvNetInput * cni = (ConvNetInput *) param;
    int n = cni->convImageDim;

    printf("convImageDim: %d\n", n);
    printf("Image input:\n");
    
    for(int i=0; i<n; i++){
        for(int j = 0; j<n; j++){
            printf("%d ", cni->convImage[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    int m = cni->convKernelDim;
    printf("convKernelDim: %d\n", m);
    printf("Kernel input:\n");
    
    for(int i=0; i<m; i++){
        for(int j = 0; j<m; j++){
            printf("%d ", cni->convKernel[i][j]);
        }
        printf("\n");
    }
    printf("\nconvStride: %d\n\n", cni->convStride);

    printf("fullyConnWeightsNum: %d\n", cni->fullyConnWeightsNum);
    printf("fullyConnWeights:\n");

    for(int i=0; i<cni->fullyConnWeightsNum; i++){
        printf("%d ", cni->fullyConnWeights[i]);
    }
    printf("\n\n----------------------\n\n");

    pthread_exit(0);
}

void * runner_print_CL(void * param){
    
    // TAKING USER INPUT FOR STATE CHANGE
    
    char option;
    printf("Thread is currently joinable, should it be converted to detached state? (Y/n): ");
    scanf("%c", &option);
    int status = 1;
    if(option == 'Y' || option == 'y'){
        status = pthread_detach(pthread_self());
    }
    if(status == 0){
        printf("Thread has been converted to detached state.\n");
    } else {
        printf("Thread has continued to stay joinable.\n");
    }
    
    cl_print_param * param_print_cl = (cl_print_param *) param;
    int n = param_print_cl->n;

    printf("\nConvolutional Output\n");
    printf("Output dimension: %dx%d\n", n, n);
    printf("Output Matrix:\n");

    for(int i=0; i<n; i++){
        for(int j = 0; j<n; j++){
            printf("%d ", param_print_cl->CLOut[i][j]);
        }
        printf("\n");
    }
    printf("\n");


    pthread_exit(0);
}

void * runner_CL_layer(void * param){
    
    cl_param * param_cl = (cl_param *) param;

    int convKernelDim = param_cl->cni->convKernelDim;
    int ** convImage = param_cl->cni->convImage;
    int ** convKernel = param_cl->cni->convKernel;
    int row = param_cl->row;
    int col = param_cl->col;

    int * res = (int *) malloc(sizeof(int));
    *res = 0;
    for(int i = 0; i<convKernelDim; i++){
        for(int j = 0; j<convKernelDim; j++){
            *res += convKernel[i][j]*convImage[i+row][j+col];
        }
    }
    
    pthread_exit(res);
}

void * runner_1d(void * param){
    
    cl_1d_param * param_id_cl = (cl_1d_param *) param;

    int ** CLOut = param_id_cl->CLOut;
    int * OneDOut = param_id_cl->OneDOut;
    int n = param_id_cl->n;
    int row = param_id_cl->row;

    for(int i = 0; i<n; i++){
        OneDOut[row*n+i] = CLOut[row][i];
    }
    
    pthread_exit(NULL);
}

void * runner_y(void * param){
    
    y_param * param_y = (y_param *) param;
    int * weights = param_y->weights;
    int * OneDOut = param_y->OneDOut;
    int n = param_y->n;

    int * Y = (int *) malloc(sizeof(int));
    *Y = 0;

    for(int i= 0; i<n; i++){
        *Y += (weights[i]*OneDOut[i]);
    }

    pthread_exit(Y);
}