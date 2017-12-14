

#ifndef SORTER_CLIENT_H
#define SORTER_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>



struct store{
char* dirname;


};

typedef struct{
    char** rToken;
    char * rText;
    size_t rowL;
    size_t numc;
}row;

void mergeSort(row* target, int col, int n);
int isNum(char* string);

#endif
