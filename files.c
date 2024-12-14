#include "files.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>

bool createNode(tPos *p){
    *p = malloc(sizeof(struct tNode)); //allocates memory for a new node
    return *p != NULL; //if memory allocation was successful return true, else return false
}

bool createNodem(mPos *p){
    *p = malloc(sizeof(struct mNode)); //allocates memory for a new node
    return *p != NULL; //if memory allocation was successful return true, else return false
}

bool createNoded(dPos *p){
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
    printf("******Assigned blocks list for the process %d\n", getpid());
    if (strcmp(method, "all") == 0) {
        for (p = m; p != NULL; p = p->next) {
            if (strcmp(p->data.method, "shared") == 0) {
                // Special formatting for 'shared' method
                printf("%p %4ld %s %s (key %d) ", p->data.adress, p->data.size, p->data.date, p->data.method, p->data.kdf);
            } else {
                printf("%p %4ld %s %s ", p->data.adress, p->data.size, p->data.date, p->data.method);
            }

            // Print additional fields if present
            if (p->data.kdf != -1 && strcmp(p->data.method, "shared")) {
                printf("  %d", p->data.kdf);
            }
            if (*p->data.filename != '\0') {
                printf("%s", p->data.filename);
            }
            printf("\n");
        }
    } else {
        for (p = m; p != NULL; p = p->next) {
            if (strcmp(p->data.method, method) == 0) {
                // Print the memory block for the selected method
                if (strcmp(p->data.method, "shared") == 0) {
                    printf("%p %4ld %s %s (key %d) ", p->data.adress, p->data.size, p->data.date, p->data.method, p->data.kdf);
                } else {
                    printf("%p %4ld %s %s ", p->data.adress, p->data.size, p->data.date, p->data.method);
                }
                if (p->data.kdf != -1 && strcmp(p->data.method, "shared")) {
                    // Print the KDF value if it's not -1
                    printf("%d  ", p->data.kdf);
                }
                if (*p->data.filename != '\0') {
                    // Print filename if it's not empty
                    printf("%s ", p->data.filename);
                }
            }
            printf("\n");
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

mPos findmemfl(char *filename,tmemlist *m){
    mPos p = *m;
    while(p != NULL && strcmp(p->data.filename,filename))
        p = p->next;
    return p;
}

void closemem(mPos p, tmemlist *m) {
    mPos q;

    if (p == *m) {  // Deleting the first node
        *m = (*m)->next;
        free(p);
    } else {
        for (q = *m; q != NULL && q->next != p; q = q->next);
        if (q != NULL) {
            q->next = p->next;
            free(p);
        }
    }
}

void FreeMemList(tmemlist m){
    mPos p,q;
    p = m;
    while(m!=NULL){
        q = p;
        p=p->next;
        closemem(q,&m);
    }
}

//-----------------------------------------process list----------------------------

void createproclist(tproclist *plist){
   *plist = NULL;
}

bool isemptyproc(tproclist plist){
    return plist == NULL;
}

bool addproc(int pid,char* date,char *comline, tproclist *plist){
    pPos q = (pPos)malloc(sizeof(struct pNode)), p;
    if (q == NULL) {
        perror("Memory allocation failed");
        return false;
    }

    q->data.PID = pid;
    strncpy(q->data.date, date, sizeof(q->data.date) - 1);

    q->data.state = ACTIVE;
    strncpy(q->data.comline, comline, sizeof(q->data.comline) - 1);

    q->next = NULL;

    if (plist == NULL) {
        *plist = q;
        return true;
    }
    p = *plist;
    while (p->next != NULL) {
        p = p->next;
    }

    p->next = q;

    return true;
}

pPos findproc(int pid, tproclist *plist){
    pPos p = *plist;
    while(p != NULL && p->data.PID != pid)
        p = p->next;
    return p;
}

void freeproclist(tproclist plist){
    pPos q;
    if(plist == NULL)
        return;
    while(plist->next != NULL){
        q = plist;
        free(q);
        plist = plist->next;
    }
    free(plist);
}

char* getusr(int pid){
    char path[256];
    char line[256];
    struct stat fileStat;
    FILE *file;
    int uid = -1;

    snprintf(path,sizeof(path),"/proc/%d/status",pid);
    file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening process status file");
        return "****";
    }

    while (fgets(line, sizeof(line), file)) {
        if (lstat(path, &fileStat) == -1) { //stat gives us information aboutr the file in question
            return "error";
        }
    }
    fclose(file);

    if (uid == -1) {
        printf("Could not find UID for process %d\n", pid);
        return "****";
    }


    if (lstat(path, &fileStat) == -1) { //stat gives us information aboutr the file in question
        return "error";
    }

    struct passwd *pws = getpwuid(fileStat.st_uid);//
    if (pws == NULL) {
        fprintf(stderr, "No user found for UID: %d\n", uid);
        return strdup("UNKNOWN"); // Return a dynamically allocated fallback
    }
    else
    return strdup(pws->pw_name);
}

void printproclist(tproclist plist){
        pPos p;
        if (!isemptyproc(plist)) {
            for (p = plist; p != NULL; p = p->next) {
                printf("%d %s p=%d %s %d %s %s\n", p->data.PID,getusr(p->data.PID),getpriority(PRIO_PROCESS,p->data.PID),p->data.date,p->data.state,p->data.comline,"command_name");
            }
        }

}


//-------------------Search list-------------

dPos SearchListFirst(tdirlist list){
    return list;
}
dPos SearchListNext(dPos pos){
    return pos->next;
}

void createdirlist(tdirlist *dlist){
    *dlist = NULL;
}

bool isemptydir(tdirlist dlist){
    return dlist == NULL;
}


bool adddir(tdirectory data, tdirlist *dlist) {

    dPos q = (dPos)malloc(sizeof(*q));

    if (q == NULL) {
        return false;
    }

    q->data = data;
    q->next = NULL;

    if (*dlist == NULL) {
        *dlist = q;
        return true;
    }

    dPos p = *dlist;
    while (p->next != NULL) {
        p = p->next;
    }

    p->next = q;
    return true;
}


dPos finddir(char* dir,tdirlist *dlist) {
        dPos p = *dlist;
        while (p != NULL && strcmp(p->data.dirname, dir))
            p = p->next;
        return p;

}


void printdir(tdirlist dlist){
    dPos p;
    if(!isemptydir(dlist)){
        for (p = dlist; p != NULL; p = p->next) {
            printf("%s", p->data.dirname);
        }
    }
}

bool removedir(dPos p,tdirlist *dlist) {
    dPos q;

    if (p == *dlist) {  // Deleting the first node
        *dlist = (*dlist)->next;
        free(p);
        return true;
    } else {
        for (q = *dlist; q != NULL && q->next != p; q = q->next);
        if (q != NULL) {
            q->next = p->next;
            free(p);
            return true;
        }
    }
    return false;
}

void freedirlist(tdirlist dlist){
    dPos q;
    if(dlist == NULL)
        return;
    while(dlist->next != NULL){
        q = dlist;
        free(q);
        dlist = dlist->next;
    }
    free(dlist);
}
