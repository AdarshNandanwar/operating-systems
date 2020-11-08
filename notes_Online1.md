# Templates

## Header

```c
#ifndef _BST_HEADER
#define _BST_HEADER

#include<stdio.h>

struct node {
    int data;
    struct node* left;
    struct node* right;
};

struct result{
    int remCount;
    int attemptCount;
};

extern struct node* getMaxNode(struct node* root);

extern int search(int x, int A[], int N);

extern void printMemoryState(FILE *, int *, struct result *, int);

extern void printScore(FILE *, struct result *);

#endif
```

## MakeFile
```
exeName = exec

$(exeName): $(exeName).c cricket.o
	gcc $(exeName).c cricket.o -o $(exeName)

# module1.o: module1.c
# 	gcc -c module1.c -o module1.o

clean: 
	rm -f *.o

run: 
	./$(exeName)
```