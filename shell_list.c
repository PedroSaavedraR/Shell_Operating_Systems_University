#include "shell_list.h"



void createEmptyList (tList *L){
    *L = NULL; // sets the address pointed to by L to NULL
}

bool isEmptyList (tList L){
    return (L==NULL); // if the first element is null, it is empty
}

tPos first (tList L){
    return L;
}

tPosL last (tList L){
    tPosL p;
    for(p=L; p->next != LNULL; p=p->next);
    return p;
}

tPosL next (tPosL p, tList L){
    return p->next;
}

tPosL previous (tPosL p, tList L){
    tPosL q;
    if (p==L)
        return LNULL;
    else{
        for (q=L; q->next != p ; q=q->next);
        return q;
    }
}

bool InsertItem(tItem d,tList L){
    tPosS aux; 
    if(!createNode(&aux)){
        return false; 
    } else{
        aux->data=d; 
        aux->next = L;
        L =aux;
        return true;
    }
}
bool createNode(tPos *p){
    *p = malloc(sizeof(struct tNode)); //allocates memory for a new node
    return *p != NULL; //if memory allocation was successful return true, else return false
}

tPos findItem (tItem d, tList L){
    tPos p;
    for(p=L; (p!= NULL)&&(strcmp(d, p->data)>0); p=p->next); //the comparison depends on tItem
    if(p!= NULL && strcmp(d,p->data)==0)
        return p;
    else
        return NULL;
}

void deleteAtPosition (tPosL p, tList *L){
    tPosL q;
    if (p == *L) {
        *L = (*L) -> next;
    } else if (p->next == LNULL) {//delete last element
        for (q = *L; q->next != p; q = q->next);//find position in front of p
        q->next = LNULL;
    } else {//delete from the middle
        q = p->next;
        p->data = q->data;
        p->next = q->next;
        p = q; //get rid of the node that used to be right after p
    }
    free(p);
}

void deleteList(tList L){
   tPos p = L.next;
   while(p != NULL){
   deleteAtPositionU(L,L);
   L=p;
   p=p.next;
   }
   free(L);
}
