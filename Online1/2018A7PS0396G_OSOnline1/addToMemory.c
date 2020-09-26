#include"memInterface.h"

void addToMemory(int * BobMem, struct result * res, int K, int num){
    res->attemptCount++;
    int i;
    for(i = 0; i<K; i++){
        if(BobMem[i] == num){
            res->remCount++;
            break;
        }
    }
    if(i == K){
        for(int j = K-1; j>0; j--){
            BobMem[j] = BobMem[j-1];
        }
        BobMem[0] = num;
    } else {
        for(int j = i; j>0; j--){
            BobMem[j] = BobMem[j-1];
        }
        BobMem[0] = num;
    }
}