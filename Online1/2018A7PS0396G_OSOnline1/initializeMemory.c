#include<stdio.h>

int initializeMemory(FILE * fp, int * BobMem, int K){
    int check, index = 0, ele, i;
    while((check = fscanf (fp, "%d", &ele)) != EOF){
        if(index>=K) break;
        for(i = 0; i<index; i++){
            if(BobMem[i] == ele){
                break;
            }
        }
        if(i == index){
            BobMem[index] = ele;
            index++;
        }
    }
    for(i = index; i<K; i++){
        BobMem[i] = -1;
    }
    return index;
}