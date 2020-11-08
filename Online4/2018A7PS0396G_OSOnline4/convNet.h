#ifndef CONVNET_H
#define CONVNET_H

typedef struct ConvNetInput
{
    int convImageDim;
    int convKernelDim;
    int **convImage;
    int **convKernel;
    int convStride;
    int fullyConnWeightsNum;
    int *fullyConnWeights;
} ConvNetInput;

/*
	Add all the structures required for the program here
*/

typedef struct ClParam{
    ConvNetInput * cni;
    int row, col;
} cl_param;

typedef struct CLPrintParam{
    int ** CLOut;
    int n;
} cl_print_param;

typedef struct CL1DParam{
    int ** CLOut;
    int * OneDOut;
    int n;
    int row;
} cl_1d_param;

typedef struct YParam{
    int * weights;
    int * OneDOut;
    int n;
} y_param;

int convNet(char *fileName);
void getData(FILE *inputFile, ConvNetInput *ConvNetInput);

/* 
	Add declarations of all the required functions here
*/

void * runner_print_input(void * param);
void * runner_print_CL(void * param);
void * runner_CL_layer(void * param);
void * runner_1d(void * param);
void * runner_y(void * param);

#endif