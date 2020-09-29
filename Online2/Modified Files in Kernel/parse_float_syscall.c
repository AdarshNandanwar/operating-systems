#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/syscalls.h>
#include "parse_float_syscall.h"

/*
    Pre-condition:
        num is a valid float number in string format

    Post-condition:
        return float num in long format
*/

SYSCALL_DEFINE2(parse_float_syscall, char __user *, num, int, len){
    long res = 0, i = 0, j = 0, t, sign = 0, first = 0, firstBinaryLen = 0, second = 0, secondLen = 0, exponent = 0, mantissa = 0, mantissaBinaryLen = 0, capacity = 32, den = 1, zeroCountTillFirstOne = 0, firstOne = 0, tempSecond = 0;
    char numBuffer[256];
    unsigned long lengthLeft = len;
    unsigned long chunkSize = sizeof(numBuffer);



    while(lengthLeft > 0){
        if(lengthLeft < chunkSize){
            chunkSize = lengthLeft;
        }
        if(copy_from_user(numBuffer, num, chunkSize)){
            return -EFAULT;
        }
        lengthLeft = lengthLeft - chunkSize;
    }


    if(len == 0) return res;


    if(numBuffer[i] == '-') {
        // setting sign bit
        sign = (1<<31);
        i++;
    } else if(numBuffer[i] == '+'){
        i++;
    }

    // IEEE 754 representation of floating point numbers

    // seperating first and second half
    // extracting first part
    for(; i<len; i++){
        if(numBuffer[i] == '.') break;
        first = 10*first+(numBuffer[i] - '0');
    }

    i++;
    // extracting second part
    for(; i<len; i++){
        second = 10*second+(numBuffer[i] - '0');
        secondLen++;
    }


    if(first == 0 && second == 0) return sign;


    // calculating exponent
    j = first;
    while(j){
        firstBinaryLen++;
        j = (j>>1);
    }
    exponent = firstBinaryLen-1;

    // calculating mantissa
    mantissa = first;
    mantissa = (mantissa&(~(1<<(firstBinaryLen-1))));
    mantissaBinaryLen = firstBinaryLen;


    capacity = 32-1-8-(mantissaBinaryLen-1);
    den = 1;
    for(t = 0; t<secondLen; t++){
        den *= 10;
    }
    tempSecond = second;


    for(j = capacity-1; j>=0; j--){
        tempSecond *= 2;
        mantissa = (mantissa<<1);
        mantissa = (mantissa|(tempSecond/den));
        if(first == 0){
            if(tempSecond/den == 1){
                if(firstOne == 0){
                    mantissa = (mantissa&(~1));
                }
                firstOne = 1;
            } 
            if(firstOne == 0){
                zeroCountTillFirstOne++;
                j++;
            } 
        }
        tempSecond %= den;
    }


    if(first == 0){
        exponent = -(zeroCountTillFirstOne+1);
    }
    exponent += 127;
    exponent = (exponent<<23);


    res = sign | exponent | mantissa ;
    return res;
}