#include <unistd.h>
#define parse_float_syscall(str, len) (syscall(441 , str, len))

float getFloat(char * str, int len){
    int isValid = 1;
    
    // check if the string is valid
    int before = 0, after = 0, decimalCount = 0;
    for(int i = 0; i<len; i++){
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



    // Converting IEEE 754 to float

    long res = parse_float_syscall(str, len);
    float num = 0;
    float mantissa = 0;
    long exponent = 0;

    // extracting exponent
    for(int i = 30; i>=23; i--){
        exponent = exponent<<1;
        exponent = (exponent | ((res>>i)&1));
    }
    exponent -= 127;

    // extracting mantissa
    for(int i = 22; i>=0; i--){
        if((res>>i)&1) mantissa = mantissa + (((float)1.0)/(1<<(23-i)));
    }

    // obtaining float number
    num = (1+mantissa);
    if(exponent >= 0){
        num = num*(1<<exponent);
    } else {
        exponent = -exponent;
        num = num/(1<<exponent);
    }
    
    // assigning sign
    if((res>>31)&1) num *= -1;



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