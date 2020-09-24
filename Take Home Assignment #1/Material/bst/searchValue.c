#include "bst.h"

struct node* searchValue(struct node* root, int val) {
    if (!root) return root;
    struct node* curr = root;
    while (1) {
        if (curr->data == val) {
            return curr;
        } else if (curr->data < val) {
            if (!curr->right) return curr->right;
            curr = curr->right;
        } else {
            if (!curr->left) return curr->left;
            curr = curr->left;
        }
    } 
}