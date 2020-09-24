#include "bst.h"
#include <stdlib.h>
#include <stdio.h>

struct node* addNode(struct node* root, int val) {
    if (root == NULL) {
        root = (struct node*)malloc(sizeof(struct node));
        root->data = val;
        root->left = NULL;
        root->right = NULL;
        return root;
    }
    struct node* curr = root;
    while (1) {
        if (curr->data > val) {
            if (curr->left == NULL) {
                curr->left = (struct node*)malloc(sizeof(struct node));
                curr = curr->left;
                curr->data = val;
                curr->left = NULL;
                curr->right = NULL;
                break;
            } else {
                curr = curr->left;
            }
        } else {
            if (curr->right == NULL) {
                curr->right = (struct node*)malloc(sizeof(struct node));
                curr = curr->right;
                curr->data = val;
                curr->left = NULL;
                curr->right = NULL;
                break;
            } else {
                curr = curr->right;
            }
        }
    }
    return root;
}