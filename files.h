//
// Created by pedro on 26/09/2024.
//

#ifndef S0_P0_FILES_H
#define S0_P0_FILES_H
#define MAX 2048
#include <stdbool.h>

typedef struct file{
    char filename[MAX];
    int mode;
    int descriptor;
}tfile;

typedef struct tNode* tPos;

struct tNode{
    tfile data;
    tPos next;
};

typedef tPos tfilelist;

bool isemptyfiles(tfilelist);
void createfilelist(tfilelist*);
void printopenfiles(tfilelist);
bool addfile(char *filename,int df, int mode,tfilelist*);
void closefile(tfilelist, tfilelist*);
tfile getfile (tPos p, tfilelist);
void cd();
tPos first (tfilelist);
tPos last (tfilelist);
tPos previous (tPos, tfilelist);
void updatefile (tfile, tPos, tfilelist*);

#endif //S0_P0_FILES_H
