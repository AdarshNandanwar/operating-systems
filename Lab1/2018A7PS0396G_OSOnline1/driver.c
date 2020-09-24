#include<stdio.h>
#include<stdlib.h>
#include"memInterface.h"
#include"memInterfacefunc.h"

int main(int argc, char * argv[]){
    if(argc != 5) {
        printf("Pass K, input_file_0, input_file_1 and output_file as parameters!\n");
        return 0;
    }

    int K = atoi(argv[1]);
    if(K<2 || 5<K){
        printf("ERROR [Invalid value of K]\n");
        return 0;
    }


    int * BobMem = (int *)malloc(K*sizeof(int));
    FILE * fin0 = fopen(argv[2], "r");
    if(fin0){
        initializeMemory(fin0, BobMem, K);
    } else {
        printf("ERROR [input0.txt could not be opened]\n");
        return 0;
    }
    fclose(fin0);

    // for(int i = 0; i<K; i++){
    //     printf("%d ", BobMem[i]);
    // }
    // printf("\n");



    FILE * fin1 = fopen(argv[3], "r");
    if(!fin1){
        printf("ERROR [input1.txt could not be opened]\n");
        return 0;
    }


    FILE * fout0 = fopen(argv[4], "w");
    if(!fout0){
        printf("ERROR [output0.txt could not be opened]\n");
        return 0;
    }
    memTest(fin1, fout0, BobMem, K);

    fclose(fin1);
    fclose(fout0);
    
    return 0;
}