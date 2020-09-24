#include "bst.h"

struct node* getMinNode(struct node* root) {
    if (!root) return root;
    struct node* curr = root;
    while (curr->left) {
        curr = curr->left;
    }
    return curr;
}