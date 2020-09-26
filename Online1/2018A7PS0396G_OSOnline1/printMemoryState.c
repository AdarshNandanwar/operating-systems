#include<stdio.h>
#include"memInterface.h"

void printMemoryState(FILE * fp2, int * BobMem, struct result * res, int K){
    char * str1 = "Last attempt was:";
    char * str2 = "Current state is:";
    fprintf(fp2, "%s%d\n", str1, res->attemptCount);
    fprintf(fp2, "%s", str2);
    for(int i = 0; i<K; i++){
        fprintf(fp2, " %d", BobMem[i]);
    }
    fprintf(fp2, "\n");
}