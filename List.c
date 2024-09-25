#include <stdlib.h>
#include <string.h>
#include "List.h"
#include <stdio.h>
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

int ClearHistoric (HLIST *l){
    int i;
    for (i=0; i<l->counter;i++)
        free(l->command[i]);
}

void PrintHistoric (HLIST *l){
    int i;
    for(i=0; i<l->counter;i++)
        printf("%d->%s\n", i, l->command[i]);


}