#include <time.h>
#include <stdio.h>
#include "shell_list.h"
#include "shell_list.c"


void authors (char *c) {
    char *s[MAX];
    int b = BreakLine(c, s);
    if (b == 1 && strcmp(s[0], "authors") == 0) {
        printf("Jose Martinez Estevez jose.martinez.estevez\nPedro Saavedra Rubinos pedro.saavedra.rubinos");
    }
    else if (b == 2) {
        if (strcmp(s[1], "-l") == 0)
            printf("jose.martinez.estevez\npedro.saavedra.rubinos");
        if (strcmp(s[1], "-n") == 0)
            printf("Jose Martinez Estevez\nPedro Saavedra Rubinos");
    }
    else
        printf("ERROR");
    }

void help (char *c, tList *coms) {
    char *s[MAX];
    int b = BreakLine(c, s);
    if (b == 1 && strcmp(s[0], "help") == 0) {
        printf(""); //Print the whole command list
    }
    if (b == 2) {
        for (tPos l = first(*coms);l->next != NULL;l = l->next) {
        if (strcmp(s[1], l->data.command) == 0)
            printf("%s", l->data.help);
    }
    }
}
/*
void date (char c){
    time_t time_now;
    time_now = localtime()

}

void infosys () {

}
*/

void chdir (char *tr[]){
    if(tr[0]==NULL)
        printf("%s\n", getcwd(actualdir, MAX));
    else
        if(chdir())

}


