#include "search.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    
    int i, pos, x, N, A[50];
    
    printf("Enter the number of elements in the array\n");
    scanf("%d", &N);

    printf("Enter the elements\n");
    for (i = 0; i < N; i++) {
        scanf("%d", &A[i]);
    }

    printf("Enter element to be searched\n");
    scanf("%d", &x);

    pos = search(x, A, N);
    
    if (pos >= 0) {
        printf("The value %d was found at position %d\n", x, pos);
    } else {
        printf("The value %d was not found in the array\n", x);
    }

    return 0;
}