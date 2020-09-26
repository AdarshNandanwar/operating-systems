#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/syscalls.h>
#include "add_syscall.h"

/*
    Pre-condition:
        num1 >= 0 num2 >= 0
        num1 + num2 < 1000

    Post-condition:
        num1+num2
*/

SYSCALL_DEFINE2(add_syscall, int, num1, int, num2){
    if(num1 < 0 && num2 < 0)
        return 1001;
    else if(num1 < 0)
        return 1002;
    else if(num2 < 0)
        return 1003;
    else if(num1+num2 >= 1000)
        return 1004;
    else 
        return num1 + num2;
}