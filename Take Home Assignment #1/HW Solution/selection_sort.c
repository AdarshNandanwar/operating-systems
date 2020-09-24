void selection_sort(int * arr, int n){
    int minInd, temp;
    for(int i = 0; i<n; i++){
        minInd = i;
        for(int j = i+1; j<n; j++){
            if(*(arr+minInd) > *(arr+j)){
                minInd = j;
            }
        }
        if(*(arr+i) > *(arr+minInd)){
            temp = *(arr+minInd);
            *(arr+minInd) = *(arr+i);
            *(arr+i) = temp;
        }
    }
}