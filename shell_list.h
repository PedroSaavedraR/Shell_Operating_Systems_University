

#ifndef S0_P0_SHELL_LIST_H
#define S0_P0_SHELL_LIST_H

#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#define MAX 2048

typedef struct tItem{
    char com[MAX];
    void (*command)();
} tItem;

typedef struct tNode* tPos;

struct tNode{
    struct tItem data;
    tPos next; //Pointer to a node
};

//Represents a list of users
typedef tPos tList;

void createEmptyList (tList* L);

bool isEmptyList (tList L);

tPos first (tList L);

tPos last (tList L);

tPos previous (tPos p, tList L);

tPos next (tPos p, tList L);

bool insertItem (tItem d, tList* L);

bool createNode(tPos *p);

void deleteAtPositionU (tPos p, tList* L);

tPos findItem (char *d , tList L);

tItem getItem(tPos p,tList L);

void deleteList(tList L);

#endif //S0_P0_SHELL_LIST_H
