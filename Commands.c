#include <time.h>
#include <stdio.h>
#include "shell_list.h"
#include "shell_list.c"
#include <sys/utsname.h>


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


/*
void date (char c){
    time_t time_now;
    time_now = localtime()

}

void infosys () {

}
*/

