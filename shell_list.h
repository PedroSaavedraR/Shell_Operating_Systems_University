

#ifndef S0_P0_SHELL_LIST_H
#define S0_P0_SHELL_LIST_H

#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "Commands.c"
#include "Commands.h"
#define MAX 2048

typedef struct tItem{
    char command[MAX];
    void (*cmd)();
    char help[MAX];
} tItem;

struct tItem commands[15] ={
        {"authors",authors,"Prints the names and logins of the program authors. authors-l prints only the logins and authors-n prints only the names"},
        {"pid", pid, "Prints the pid of the process executing the shell."},
        {"ppid",ppid,"Prints the pid of the shell’s parent process."},
        {"cd", cd, "Changes the current working directory of the shell to dir (using the chdir system call). When invoked without arguments it prints the current working directory (using the getcwd system call"},
        {"date", date, "Prints the current date in the format DD/MM/YYYY and the current time in the format hh:mm:ss"},
        {"historic",historic,"Shows the historic of commands executed by this shell.\n– historic Prints all the comands that have been input with their order number\n– historic N Repeats command number N (from historic list)\n– historic -N Prints only the lastN comands"},
        {"open",open,"Opens a file and adds it (together with the file descriptor and the opening mode to the list of shell open files. Open without arguments lists the shell open files. For each file it lists its descriptor, the file name and the opening mode."},
        {"close",close,"Closes the df file descriptor and eliminates the corresponding item from the list"},
        {"dupe",dup,"Duplicates the df file descriptor (using the dup system call, creating the corresponding new entry on the file list"},
        {"infosys",infosys,"Prints information on the machine running the shell (as obtained via the uname system call/library function)"},
        {"help",help," help displays a list of available commands. help cmd gives a brief help on the usage of comand cmd"},
        {"quit",quit,"Ends the shell"},
        {"Exit",exit,"Ends the shell"},
        {"bye",bye,"Ends the shell"},
};




typedef struct tNode* tPos;

struct tNode{
    struct tItem data;
    tPos next; //Pointer to a node
};

//Represents a list of users
typedef tPos tList;

void createEmptyList (tList* L);

bool isEmptyList (tList L);

tPos first (tList L);

tPos last (tList L);

tPos previous (tPos p, tList L);

tPos next (tPos p, tList L);

bool insertItem (tItem d, tList* L);

bool createNode(tPos *p);

void deleteAtPositionU (tPos p, tList* L);

tPos findItem (char *d , tList L);

tItem getItem(tPos p,tList L);

void deleteList(tList L);

#endif //S0_P0_SHELL_LIST_H
