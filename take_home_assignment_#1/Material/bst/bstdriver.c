#include "bst.h"
#include <stdio.h>


int main(int argc, char* argv[]) {
    struct node* root = NULL;
    root = addNode(root, 1);
    root = addNode(root, 2);
    root = addNode(root, 5);
    root = addNode(root, 3);
    root = addNode(root, 7);
    root = addNode(root, 6);
    root = addNode(root, 4);
    
    struct node* result;
    for (int i=0; i<=10; i++) {
        result = searchValue(root, i);
        if (result) {
            printf("The value %d was found\n", i);
        } else {
            printf("The value %d was not found\n", i);
        }
    }
    
    result = getMaxNode(root);
    printf("the maximum value is %d\n", result->data);

    root = addNode(root, 10);
    result = getMaxNode(root);
    printf("the maximum value is %d\n", result->data);

    result = getMinNode(root);
    printf("the minimum value is %d\n", result->data);

    root = addNode(root, 0);
    result = getMinNode(root);
    printf("the minimum value is %d\n", result->data);
    
    return 0;
}