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

tPos last (tList L){
    tPos p;
    for(p=L; p->next != NULL; p=p->next);
    return p;
}

tPos next (tPos p, tList L){
    return p->next;
}

tPos previous (tPos p, tList L){
    tPos q;
    if (p==L)
        return NULL;
    else{
        for (q=L; q->next != p ; q=q->next);
        return q;
    }
}

bool InsertItem(tItem d,tList L){
    tPos aux;
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

tPos findItem (char *d, tList L){
    tPos p;
    for(p=L; (p!= NULL)&&(strcmp(d, p->data.com)>0); p=p->next); //the comparison depends on tItem
    if(p!= NULL && strcmp(d,p->data.com)==0)
        return p;
    else
        return NULL;
}

tItem getItem (tPos p, tList L){
    return p->data;
}

int BreakLine(char *lin,char *pz[]){
    int i;

    if((pz[0]=strtok(lin," \t\n"))==NULL)
        return 0;
    i=1;
    while((pz[i]=strtok(NULL," \t\n"))!=NULL)
        i++;
    return i;
}
void deleteAtPosition (tPos p, tList *L){
    tPos q;
    if (p == *L) {
        *L = (*L) -> next;
    } else if (p->next == NULL) {//delete last element
        for (q = *L; q->next != p; q = q->next);//find position in front of p
        q->next = NULL;
    } else {//delete from the middle
        q = p->next;
        p->data = q->data;
        p->next = q->next;
        p = q; //get rid of the node that used to be right after p
    }
    free(p);
}
/*
void deleteList(tList L){
   tPos p = L->next;
   while(p != NULL){
   deleteAtPositionU(L,L);
   L=p;
   p=p->next;
   }
   free(L);
}
*/