void insertion_sort(int * arr, int n){
    int i = 1, j = 0, temp;
    for(int i = 0; i<n; i++){
        temp = arr[i];
        j = i-1;
        while(j>=0 && arr[j]>temp){
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = temp;
    }
}