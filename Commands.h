#ifndef S0_P0_COMMANDS_H
#define S0_P0_COMMANDS_H

#include "shell_list.h"

//Prints the names and logins of the program authors. Authors-l prints only the logins and Authors-n prints only the names
void authors (char*);
void pid(char*);
void ppid(char*);
void cd(char*);
void date (char *c);
void historic(char*);
void open(char*);
void close(char*);
void dup(char*);
void infosys(char*);
void quit(char*);
void Exit(char*);
void bye(char*);

#endif //S0_P0_COMMANDS_H
