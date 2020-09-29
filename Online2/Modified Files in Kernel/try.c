#include<stdio.h>
#include<string.h>


void printBinary(long n){
    for(int i = 31; i>=0; i--){
        printf("%ld", ((n>>i)&1));
    }
    printf("\n");
}

long power(long n, long x){
    if(x == 0) return 1;
    if(x == 1) return n;
    return power(n, x/2)*power(n, x-x/2);
}

long ieee754(char * num, int len){
    long res = 0, i = 0, j, sign = 0, first = 0, firstBinaryLen = 0, second = 0, secondLen = 0, exponent = 0, mantissa = 0, mantissaBinaryLen = 0, capacity = 32-1-8-(mantissaBinaryLen-1), den = power(10, secondLen), zeroCountTillFirstOne = 0, firstOne = 0, tempSecond = second;

    if(len == 0) return res;

    if(num[i] == '-') {
        // setting sign bit
        sign = (1<<31);
        i++;
    } else if(num[i] == '+'){
        i++;
    }

    // IEEE 754 representation of floating point numbers

    // seperating first and second half

    // extracting first part
    for(; i<len; i++){
        if(num[i] == '.') break;
        first = 10*first+(num[i] - '0');
    }
        
    // printf("First: %ld\n", first);
    // printBinary(first);
    // printf("\n");

    i++;
    // extracting second part
    for(; i<len; i++){
        second = 10*second+(num[i] - '0');
        secondLen++;
    }



    // printf("Second: %ld\n", second);
    // printBinary(second);
    // printf("\n");


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

    // printf("Mantissa: %ld\n", firstBinaryLen);


    // printf("Capacity: %ld\n", capacity);
    // printf("Den: %ld\n", den);
    for(j = capacity-1; j>=0; j--){
        tempSecond *= 2;
        // printf("step %ld: %ld\n", j, tempSecond);
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
    
    // printf("Mantissa: \n");
    // printBinary(mantissa);
    // printf("\n");

    // printf("Exponent: \n");
    // printBinary(exponent);
    // printf("\n");


    res = sign | exponent | mantissa ;

    return res;
}

int main(){
    char * num;
    scanf("%s", num);
    long res = ieee754(num, strlen(num));
    printf("Response: ");
    printBinary(res);
    return 0;
}