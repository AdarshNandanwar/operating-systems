#ifndef MEMTEST_HEADER
#define MEMTEST_HEADER

#include<stdio.h>

extern int initializeMemory(FILE *, int *, int);

extern int memTest(FILE *, FILE *, int *, int);

extern void addToMemory(int *, struct result *, int, int);

extern void printMemoryState(FILE *, int *, struct result *, int);

extern void printScore(FILE *, struct result *);

#endif