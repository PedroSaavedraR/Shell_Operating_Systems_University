//
// Created by pedro on 26/09/2024.
// //

#ifndef S0_P0_FILES_H
#define S0_P0_FILES_H
#define MAX 2048
#include <stdbool.h>

enum status {FINISHED , STOPPED , SIGNALED, ACTIVE};

typedef struct file{
    char filename[MAX];
    int mode;
    int descriptor;
}tfile;

typedef struct memory{
    void *adress;
    long size;
    char date[MAX];
    char method[MAX];
    int kdf;
    char filename[MAX];
}tmemory;

typedef struct process{
    int PID;
    char date[MAX];
    enum status state;
    char comline[MAX];
    int priority;
}tprocess;



typedef struct tNode* tPos;
typedef struct mNode* mPos;
typedef struct pNode* pPos;

struct tNode{
    tfile data;
    tPos next;
};

struct mNode{
    tmemory data;
    mPos next;
};

struct pNode{
    tprocess data;
    pPos next;
};

typedef tPos tfilelist;
typedef mPos tmemlist;
typedef pPos tproclist;

bool initfilelist(tfilelist* list);
bool isemptyfiles(tfilelist);
void createfilelist(tfilelist*);
void printopenfiles(tfilelist);
bool addfile(char *filename,int df, int mode,tfilelist*);
void closefile(tPos, tfilelist*);
tfile getfile (tPos p, tfilelist);
tPos findfile (int ,tfilelist);
void cd();
tPos first (tfilelist);
tPos last (tfilelist);
tPos previous (tPos, tfilelist);
void updatefile (tfile, tPos, tfilelist*);
void FreeFileList(tfilelist);

bool isemptymem(tmemlist);
void creatememlist(tmemlist*);
void printmemory(tmemlist, char*);

bool addmem(void *address,long size,char* date,char *method,char *filename,int kdf, tmemlist*);
mPos findmemad(void* adress,tmemlist*);
mPos findmemsz(int size,tmemlist*);
mPos findmemsh(int key,tmemlist*);
mPos findmemfl(char* filename,tmemlist*);
void closemem(mPos,tmemlist*);
void FreeMemList(tmemlist);

void createproclist(tproclist*);
bool isemptyproc(tproclist);
bool addproc(tprocess,tproclist*);
pPos findproc(int pid, tproclist*);
void freeproclist(tproclist);

#endif //S0_P0_FILES_H
