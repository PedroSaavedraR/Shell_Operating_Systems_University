#include "files.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>

bool createNode(tPos *p){
    *p = malloc(sizeof(struct tNode)); //allocates memory for a new node
    return *p != NULL; //if memory allocation was successful return true, else return false
}


void createfilelist (tfilelist *l){
    *l = NULL;
}

bool isemptyfiles (tfilelist L){
    return (L==NULL); // if the first element is null, it is empty
}

bool addfile(char *filename, int df, int mode, tfilelist *L) {
    tPos q, p;

    // Create a new node
    if (!createNode(&q)) // if malloc failed
        return false;

    // Initialize the new node's data
    strcpy(q->data.filename, filename);
    q->data.descriptor = df;
    q->data.mode = mode;
    q->next = NULL;

    // If the list is empty, insert at the front
    if (*L == NULL) {
        *L = q;
    }
    //Inserting at the end
    else{
        p = *L;
        while(p->next != NULL && p->next->data.descriptor < df){
            p=p->next;
        }
        q->next = p->next;
        p->next = q;
    }

    return true;
}

tPos findfile(int df, tfilelist L) {
    tPos p = L;
    int i = 0;

    while (p != NULL) {
        if (p->data.descriptor == df) {
            return p;
        }
        p = p->next;
        i++;
    }
    return NULL;
}


void closefile (tPos p, tfilelist* L) {
    tPos q;

    if(p==*L) //delete the first
        *L = (*L)->next;
    else if (p->next == NULL) { // Deleting the last element
        for (q = *L; q->next != p; q = q->next);
        q->next = NULL;
    } else { //deleting from the middle
        q=p->next;
        p->data = q->data;
        p->next = q->next;
        p=q; //get rid of the duplicated node

    }
    free(p);
}

bool initfilelist(tfilelist *L){
    return (addfile("stdin", 0, fcntl(0,F_GETFL), L)  &&  addfile("stdout", 1, fcntl(1,F_GETFL), L) && addfile("stderr", 2, fcntl(2,F_GETFL), L));
}

void printopenfiles(tfilelist l) {
    tPos p;
    if (!isemptyfiles(l)) {
        for (p = l; p != NULL; p = p->next) {
            printf("Name: %s\tDescriptor: %d\tMode: %d\n", p->data.filename, p->data.descriptor, p->data.mode);
        }
    } else printf("File list is empty");
}

void FreeFileList(tfilelist l) {
    tPos p;
    while (l != NULL) {
        p = l;
        l = l->next;
        free(p);
    }
}
// ----------------------------------------memory list------------------------------------------------

void creatememlist (tmemlist *m){
    *m = NULL;
}

bool isemptymem (tmemlist m){
    return m == NULL;
}


void printmemory(tmemlist m, char *method) {//bhb
    mPos p;
    if (strcmp(method, "all") == 0) {
        for (p = m; p != NULL; p = p->next)
            printf("%s %d %s %s %d %s", p->data.adress, p->data.size, p->data.date, p->data.method,p->data.kdf,p->data.filename);
    } else {
        for (p = m; p != NULL; p = p->next) {
            if (strcmp(p->data.method, method) == 0)
                printf("%s %d %s %s %d %s", p->data.adress, p->data.size, p->data.date, p->data.method,p->data.kdf,p->data.filename);
        }
    }
}



bool addmem (char adress,int size,char *date,char *method,char *filename,int kdf,tmemlist *m){
    mPos q, p;

    // Create a new node
    if (!createNode(&q)) // if malloc failed
        return false;

    // Initialize the new node's data
    strcpy(q->data.adress, &adress);
    strcpy(q->data.method, method);
    strcpy(q->data.date,date);
    strcpy(q->data.filename,filename);
    q->data.size = size;q->data.kdf = kdf;
    q->next = NULL;

    p = *m;
    while(p->next != NULL){
        p=p->next;
        }
    q->next = p->next;
    p->next = q;

    return true;
}

mPos findmemad(char *adress,tmemlist *m) {
    mPos p = *m;
    while(p != NULL && p->data.adress != adress);
    return p;
}

mPos findmemsz(int size,tmemlist *m){
    mPos p = *m;
    while(p != NULL && p->data.size != size);
    return p;
}

void closemem(mPos p,tmemlist* m){
    mPos q;

    if(p==*m) //delete the first
        *m = (*m)->next;
    else if (p->next == NULL) { // Deleting the last element
        for (q = *m; q->next != p; q = q->next);
        q->next = NULL;
    } else { //deleting from the middle
        q=p->next;
        p->data = q->data;
        p->next = q->next;
        p=q; //get rid of the duplicated node

    }
    free(p);
}

void FreeMemList(tmemlist m){
    mPos p,q;
    p = m;
    while(m!=NULL){
        q = p;
        free(q);
        p=p->next;
    }
}
