#include <unistd.h>
#include <stdio.h>
#define parse_float_syscall(str, len) (syscall(440 , str, len))

typedef union { 
    float val; 
    struct
    {
        unsigned int mantissa : 23; 
        unsigned int exponent : 8; 
        unsigned int sign : 1; 
  
    } floatBits; 
} floatUnion; 

void printBinary(long n){
    for(int i = 31; i>=0; i--){
        printf("%ld", ((n>>i)&1));
    }
    printf("\n");
}

float getFloat(char * str, int len){
    int isValid = 1;
    
    // check if the string is valid
    int before = 0, after = 0, decimalCount = 0;
    for(int i = 0; i<len; i++){
        // if(str[i] == '-' || str[i] == '+'){
        if(str[i] == '-'){
            if(i != 0){
                isValid = 0;
                break;
            }
            continue;
        }
        if(str[i] == '.'){
            decimalCount++;
            if(decimalCount > 1){
                isValid = 0;
                break;
            }
            continue;
        } 
        if('0' <= str[i] && str[i] <= '9'){
            if(decimalCount == 0){
                before++;
            } else {
                after++;
            }
        } else {
            isValid = 0;
            break;
        }
    }

    if(before == 0) isValid = 0;
    if(decimalCount > 0 && after == 0) isValid = 0;


    if(!isValid) return 100001.0;


    long res = parse_float_syscall(str, len);
    float num;

    // Converting IEEE 754 to float



    // Method 1 : Manual Extraction

    num = 0;
    float mantissa1 = 0;
    long exponent1 = 0;

    // extracting exponent
    for(int i = 30; i>=23; i--){
        exponent1 = exponent1<<1;
        exponent1 = (exponent1 | ((res>>i)&1));
    }
    exponent1 -= 127;

    // extracting mantissa
    for(int i = 22; i>=0; i--){
        if((res>>i)&1) mantissa1 = mantissa1 + (((float)1.0)/(1<<(23-i)));
    }

    // obtaining float number
    num = (1+mantissa1);
    if(exponent1 >= 0){
        num = num*(1<<exponent1);
    } else {
        exponent1 = -exponent1;
        num = num/(1<<exponent1);
    }
    
    // assigning sign
    if((res>>31)&1) num *= -1;

    printf("Method 1: %f\n", num);
    printf("Exponent: %ld\n", exponent1);
    printBinary(exponent1);




    // Method 2 : Union

    floatUnion tempNum;
    num = 0;
    long exponent2 = 0, mantissa2 = 0;
    
    // extracting exponent
    for(int i = 30; i>=23; i--){
        exponent2 = exponent2<<1;
        exponent2 = (exponent2 | ((res>>i)&1));
    }
    tempNum.floatBits.exponent = exponent2;
    
    // extracting mantissa
    for(int i = 22; i>=0; i--){
        mantissa2 = mantissa2<<1;
        mantissa2 = (mantissa2 | ((res>>i)&1));
    }
    tempNum.floatBits.mantissa = mantissa2;

    // assigning sign
    if((res>>31)&1) num *= -1;

    num = tempNum.val;
    printf("Method 2: %f\n", num);






    if(num > 100000.0){
        return 100002.0;
    } else if(-0.01 < num && num < 0.01){
        return 100003.0;
    } else if(num < -100000.0){
        return 100004.0;
    } else {
        return num;
    }
}