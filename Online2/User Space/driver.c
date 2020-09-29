#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include "stringToFloat.h"

int main(int argc, char * argv[])
{
    char str[50];
    printf("Enter a number: ");
    scanf(" %s", str);
    int len = strlen(str);
    float res = getFloat(str, len);
    printf("Float is : %.12f\n", res);
    return 0;
}