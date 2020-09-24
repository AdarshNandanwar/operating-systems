void quick_sort(int * arr, int n){
    for(int i = n-2; i>=0; i--){
        for(int j = i+1; j<n; j++){
            if(*(arr+j-1) > *(arr+j)){
                int temp = *(arr+j);
                *(arr+j) = *(arr+j-1);
                *(arr+j-1) = temp;
            }
        }
    }
}