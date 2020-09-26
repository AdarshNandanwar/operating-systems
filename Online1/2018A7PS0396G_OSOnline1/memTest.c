#include<stdlib.h>
#include"memInterface.h"
#include"memInterfacefunc.h"

int memTest(FILE * fp1, FILE * fp2, int * BobMem, int K){
    struct result * res = (struct result *)malloc(sizeof(struct result));
    res->remCount = 0;
    res->attemptCount = 0;
    int inp, check;
    // while(fscanf(fp1, "%d", &inp)){
    while(( check = fscanf (fp1, "%d", &inp)) != EOF){
        // printf("%d ", inp);
        if(inp == 1){
            fscanf(fp1, "%d", &inp);
            // printf("%d\n ", inp);
            if(inp >= 1 || inp <= 200){
                addToMemory(BobMem, res, K, inp);
            }
        } else if(inp == 2){
            printMemoryState(fp2, BobMem, res, K);
        } else if(inp == 3){
            printScore(fp2, res);
        } else {
            char * str1 = "Last attempt number was:";
            char * str2 = " Error[queryHandler fails]:Unknown Query Type.";
            fprintf(fp2, "%s%d%s\n", str1, res->attemptCount, str2);
        }
        // printf("\n");
    }
    char * str1 = "Final Score: ";
    char * str2 = " and Attempts: ";
    fprintf(fp2, "%s%d%s%d\n", str1, res->remCount, str2, res->attemptCount);
}