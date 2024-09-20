#include <time.h>
#include <stdio.h>
#include "shell_list.h"
#include "shell_list.c"

void authors (char c){
    int i=0;
    if (BreakLine(c,)==0)
        printf("Jose Martinez Estevez jose.martinez.estevez\nPedro Saavedra Rubinos pedro.saavedra.rubinos");
    else
        if (strcmp(*pc[1],"-l")==0)
            printf("jose.martinez.estevez\npedro.saavedra.rubinos");
        else if (strcmp(*pc[1],"-n")==0)
            printf("Jose Martinez Estevez\nPedro Saavedra Rubinos");
        else
            printf("ERROR");
}

void date (char c){
    time_t time_now;
    time_now = localtime()

}