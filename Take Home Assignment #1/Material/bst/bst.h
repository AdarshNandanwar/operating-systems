#ifndef _BST_HEADER
#define _BST_HEADER

struct node {
    int data;
    struct node* left;
    struct node* right;
};

/*
    Pre-condition:
        root is the current root of the BST
        val is the value to be added to the BST
        val doesn't match the data of any node in the tree

    Post-condition:
        root of the tree after adding a node at the correct position 
        whose data is val
*/
extern struct node* addNode(struct node* root, int val);


/*
    Pre-condition:
        root is the current root of the BST
        val is the value to be searched in the BST

    Post-condition:
        returns NULL if value is not found
        otherwise returns a node whose data is val
*/        
extern struct node* searchValue(struct node* root, int val);

/*
    Pre-condition:
        root is the root of the BST
    
    Post-condition:
        return NULL if the tree is empty
        otherwise returns the node whose data value is minimum
*/
extern struct node* getMinNode(struct node* root);

/*
    Pre-condition:
        root is the root of the BST
    
    Post-condition:
        return NULL if the tree is empty
        otherwise returns the node whose data value is maximum
*/
extern struct node* getMaxNode(struct node* root);

#endif