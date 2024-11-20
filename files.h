//
// Created by pedro on 26/09/2024.
// //

#ifndef S0_P0_FILES_H
#define S0_P0_FILES_H
#define MAX 2048
#include <stdbool.h>

typedef struct file{
    char filename[MAX];
    int mode;
    int descriptor;
}tfile;

typedef struct memory{
    char *adress[MAX];
    int size;
    char *date[MAX];
    char *method[MAX];
    int kdf;
    char *filename[MAX];
}tmemory;

typedef struct tNode* tPos;
typedef struct mNode* mPos;

struct tNode{
    tfile data;
    tPos next;
};

struct mNode{
    tmemory data;
    mPos next;
};

typedef tPos tfilelist;
typedef mPos tmemlist;

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

bool addmem(char *address,int size,char *date,char *method,char *filename,int kdf, tmemlist*);
mPos findmemad(char* adress,tmemlist*);
mPos findmemsz(int siize,tmemlist*);
void closemem(mPos,tmemlist*);
void FreeMemList(tmemlist);

#endif //S0_P0_FILES_H
