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
    *l = NULL; // sets the address pointed to by L to NULL
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
        while(p->next!=NULL){
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
