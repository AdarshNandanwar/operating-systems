#include "search.h"
#include <stdio.h>

int search(int x, int A[], int N) {

    int lo = 0, hi = N-1;
    int mid;

    while (lo <= hi) {
        mid = (lo + hi) / 2;

        if (A[mid] == x) return mid;
        if (A[mid] < x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    return -1;
}