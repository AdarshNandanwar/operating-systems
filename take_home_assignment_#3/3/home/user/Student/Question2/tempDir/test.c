#include<string.h>
#include<stdio.h>

int main(){
    char * s1 = "adarsh";
    char * s2 = " nandanwar";
    char * s3 = malloc(strlen(s1)+strlen(s2));
    strcat(s3, s1);
    strcat(s3, s2);
    printf("%s\n", s3);
}