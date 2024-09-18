#include "shell_list.h"

bool createNode(tPos *p){
    *p = malloc(sizeof(struct tNode)); //allocates memory for a new node
    return *p != NULL; //if memory allocation was successful return true, else return false
}

void createEmptyList (tList *L){
    *L = NULL; // sets the address pointed to by L to NULL
}

bool isEmptyList (tList L){
    return (L==NULL); // if the first element is null, it is empty
}

tPos first (tList L){
    return L;
}

//abcde

tPos findItem (tItem d, tList L){
    tPos p;
    for(p=L; (p!= NULL)&&(strcmp(d, p->data)>0); p=p->next); //the comparison depends on tItem
    if(p!= NULL && strcmp(d,p->data)==0)
        return p;
    else
        return NULL;
}

void deleteAtPositionU (tPos p, tList* L){}

void deleteList(tList L){
   tPos p = L.next;
   while(p != NULL){
   deleteAtPositionU(L,L);
   L=p;
   p=p.next;
   }
   free(L);
}
