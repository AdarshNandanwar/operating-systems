#include<stdio.h>
#include"memInterface.h"

void printScore(FILE * fp2, struct result * res){
    // Current Score: 3 and Attempts: 6
    char * str1 = "Current Score: ";
    char * str2 = " and Attempts: ";
    fprintf(fp2, "%s%d%s%d\n", str1, res->remCount, str2, res->attemptCount);
}
