#include "bst.h"

struct node* getMaxNode(struct node* root) {
    if (!root) return root;
    struct node* curr = root;
    while (curr->right) {
        curr = curr->right;
    }
    return curr;
}