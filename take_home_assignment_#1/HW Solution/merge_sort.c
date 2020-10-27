#include<stdlib.h>

void merger(int * arr, int st, int mid, int end){
    int i = st, j = mid+1, k = 0;
    int * temp = (int *) malloc((end-st+1)*sizeof(int));
    while(i<=mid && j<=end){
        if(arr[i] < arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while(i<=mid) temp[k++] = arr[i++];
    while(j<=end) temp[k++] = arr[j++];
    for(int i = 0; i<end-st+1; i++) arr[st+i] = temp[i];
}

void merge_helper(int * arr, int st, int end){
    if(st>=end) return;
    int mid = st+(end-st)/2;
    merge_helper(arr, st, mid);
    merge_helper(arr, mid+1, end);
    merger(arr, st, mid, end);
}

void merge_sort(int * arr, int n){
    int st = 0, end = n-1;
    merge_helper(arr, st, end);
}