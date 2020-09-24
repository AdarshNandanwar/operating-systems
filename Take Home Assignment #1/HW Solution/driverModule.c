#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"sorting_algorithms.h"

int main(int argc, char * argv[]){
    if(argc < 4) {
        printf("Pass input_file, output_file and sorting_algorithm as parameters!\n");
        return 0;
    }

    FILE * fin = fopen(argv[1], "r");
    FILE * fout = fopen(argv[2], "w");

    if(fin){
        printf("File %s opened successfully.\n", argv[1]);
    } else {
        printf("Error while opening file %s.\n", argv[1]);
        return 0;
    }
    if(fout){
        printf("File %s opened successfully.\n", argv[2]);
    } else {
        printf("Error while opening file %s.\n", argv[2]);
        return 0;
    }

    // reading the array from specified input file
    int n;
    fscanf(fin, "%d", &n);
    int * arr = (int *) malloc(n*sizeof(int));
    for(int i = 0; i<n; i++) fscanf(fin, "%d ", &arr[i]);

    // sorting the array using specified sorting algorithm
    if(strcmp(argv[3], "bubble") == 0){
        printf("Sorting using bubble sort.\n");
        bubble_sort(arr, n);
    } else if(strcmp(argv[3], "selection") == 0){
        printf("Sorting using selection sort.\n");
        selection_sort(arr, n);
    } else if(strcmp(argv[3], "insertion") == 0){
        printf("Sorting using insertion sort.\n");
        insertion_sort(arr, n);
    } else if(strcmp(argv[3], "merge") == 0){
        printf("Sorting using merge sort.\n");
        merge_sort(arr, n);
    } else if(strcmp(argv[3], "quick") == 0){
        printf("Sorting using quick sort.\n");
        printf("Quick sort is not implemented. Redirecting to merge sort.\n");
        merge_sort(arr, n);
    } else {
        printf("Invalid sorting algorithm.\n");
    }

    // saving the sorted array into specified output file
    for(int i = 0; i<n; i++){
        fprintf(fout, "%d ", arr[i]);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}