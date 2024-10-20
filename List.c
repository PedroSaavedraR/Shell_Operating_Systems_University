#include <stdlib.h>
#include <string.h>
#include "List.h"
#include <stdio.h>
#include <errno.h>
#define MAX 2048

int IsHistoricEmpty(HLIST *l){
    return l->counter==0;
}

void InitHistoric (HLIST *l){
    l->counter=0;
}

int AddHistoricElement(HLIST *l, char * cmd){
    if(l->counter == MAX-1){
        errno=ENOSPC; //error number for not enough space
        return -1;
    }
    if((l->command[l->counter]=strdup (cmd))==NULL)
        return -1;
    l->counter++;
    return l->counter;
}

void ClearHistoric (HLIST *l){
    int i;
    for (i=0; i<l->counter;i++)
        free(l->command[i]);
}

void PrintHistoric (HLIST *l){
    int i;
    for(i=0; i<l->counter;i++)
        printf("%d->%s", i , l->command[i]);
}

void PrintHistoricLast(HLIST *l, int n) {
    for (int i=l->counter-n; i<l->counter;i++) {
        printf("%d->%s", i, l->command[i]);
    }
}


char* GetHistoricElement(HLIST *l,int n){
    return l->command[n];

}

int GetLength(HLIST *l){
    return l->counter;
}