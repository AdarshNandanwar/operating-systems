#include <stdio.h>
#include <unistd.h>
#include "addWrapper.h"
#define MY_SYSCALL 440

int main(int argc, char * arv[]){
    int num1, num2, res_direct, res_wrapper;
    printf("Enter number 1 and number 2:\n");
    scanf("%d %d", &num1, &num2);
    res_direct = syscall(MY_SYSCALL, num1, num2);
    res_wrapper = add_syscall(num1, num2);

    printf("The result of adding %d with %d\n", num1, num2);
    printf("Result of Direct call is %d\n", res_direct);
    printf("Result of wrapper call is %d\n", res_wrapper);

    return 0;
}