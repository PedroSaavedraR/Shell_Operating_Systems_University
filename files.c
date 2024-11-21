#include "files.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

bool createNode(tPos *p){
    *p = malloc(sizeof(struct tNode)); //allocates memory for a new node
    return *p != NULL; //if memory allocation was successful return true, else return false
}

bool createNodem(mPos *p){
    *p = malloc(sizeof(struct mNode)); //allocates memory for a new node
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


void printmemory(tmemlist m, char *method) {
    mPos p;
    if (strcmp(method, "all") == 0) {
        printf("******Assigned blocks list for the process %d\n", getpid());
        for (p = m; p != NULL; p = p->next) {
            if (strcmp(p->data.method, "shared") == 0) {
                // Special formatting for 'shared' method
                printf("%p %4ld %s %s (key %d)\n", p->data.adress, p->data.size, p->data.date, p->data.method, p->data.kdf);
            } else {
                printf("%p %4ld %s %s\n", p->data.adress, p->data.size, p->data.date, p->data.method);
            }

            // Print additional fields if present
            if (p->data.kdf != -1) {
                printf("  %d\n", p->data.kdf);
            }
            if (*p->data.filename != '\0') {
                printf("%s\n", p->data.filename);
            }
        }
    } else {
        for (p = m; p != NULL; p = p->next) {
            if (strcmp(p->data.method, method) == 0) {
                // Print the memory block for the selected method
                if (strcmp(p->data.method, "shared") == 0) {
                    printf("%p %4ld %s %s (key %d)\n", p->data.adress, p->data.size, p->data.date, p->data.method, p->data.kdf);
                } else {
                    printf("%p %4ld %s %s\n", p->data.adress, p->data.size, p->data.date, p->data.method);
                }
                if (p->data.kdf != -1) {
                    // Print the KDF value if it's not -1
                    printf("  %d\n", p->data.kdf);
                }
                if (*p->data.filename != '\0') {
                    // Print filename if it's not empty
                    printf("%s\n", p->data.filename);
                }
            }
        }
    }
}

bool addmem(void *address, long size,char* date,char *method, char *filename, int kdf, tmemlist *m) {
    mPos q, p;
    if(address == NULL)
        return false;

    if (!createNodem(&q) || q == NULL) {
        fprintf(stderr, "Error: Could not create node\n");
        return false;
    }

    if (method != NULL) {
        strncpy(q->data.method,method,MAX-1);
        q->data.method[MAX - 1] = '\0';
    } else {
        q->data.method[0] = '\0';
    }

    if (date != NULL) {
        strncpy(q->data.date, date, MAX - 1);
        q->data.date[MAX - 1] = '\0';
    } else {
        q->data.date[0] = '\0';
    }

    if (filename != NULL) {
        strncpy(q->data.filename, filename, MAX - 1);
        q->data.filename[MAX - 1] = '\0';
    } else {
        q->data.filename[0] = '\0';
    }

    q->data.adress = address;
    q->data.size = size;
    q->data.kdf = kdf == 0 ? -1 : kdf;
    q->next = NULL;

    if (*m == NULL) {
        *m = q;
        return true;
    }
    p = *m;
    while (p->next != NULL) {
        p = p->next;
    }

    p->next = q;

    return true;
}

mPos findmemad(void* adress,tmemlist *m) {
    mPos p = *m;
    while(p != NULL && p->data.adress != adress){
        p = p->next;}
    return p;
}

mPos findmemsz(int size,tmemlist *m){
    mPos p = *m;
    while(p != NULL && p->data.size != (long )size)
        p = p->next;
    return p;
}

mPos findmemsh(int key,tmemlist *m){
    mPos p = *m;
    while(p != NULL && p->data.kdf != key)
        p = p->next;
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
