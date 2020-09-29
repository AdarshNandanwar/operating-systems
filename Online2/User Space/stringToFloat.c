#include <unistd.h>
#include <stdio.h>
#define parse_float_syscall(str, len) (syscall(441 , str, len))

float getFloat(char * str, int len){
    int isValid = 1;
    
    // check if the string is valid
    int before = 0, after = 0, decimalCount = 0;
    for(int i = 0; i<len; i++){
        if(str[i] == '-' || str[i] == '+'){
            if(i != 0){
                isValid = 0;
                break;
            }
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



    for(int i = 31; i>=0; i--) printf("%ld", ((res>>i)&1));
    printf("\n");




    if(res > 100000.0){
        return 100002.0;
    } else if(-0.1 < res && res < 0.1){
        return 100003.0;
    } else if(res < -100000.0){
        return 100004.0;
    } else {
        return res;
    }
}