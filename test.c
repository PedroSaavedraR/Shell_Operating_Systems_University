#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "shell_list.h"
#include "shell_list.h"
#include "Commands.c"

#define MAX 2048

int main (){

    char command[MAX] = "authors";
    authors(command); //problem when reading spaces

    return 0;
}
