#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "shell_list.h"
#include "shell_list.c"
#include "Commands.c"

#define MAX 2048

void ccomlist(tList c, tItem k[15]){
    createEmptyList(*c);
    for(int i=0;i<16;i++){
        insertItem(k[i],&c);
    }
};
int main (){
    tList c;
    tItem *k;

    ccomlist(c, k);

    char command[MAX] = "authors";
    authors(command);

    help("help authors", &c);

    return 0;
}
