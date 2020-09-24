#include "search.h"
#include <stdio.h>

int search(int x, int A[], int N) {
    int pos;
    for (pos = 0; pos < N; pos++) {
        if (A[pos] == x) {
            return pos;
        }
    }
    return -1;
}