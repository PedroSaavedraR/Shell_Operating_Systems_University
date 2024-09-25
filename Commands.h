#ifndef S0_P0_COMMANDS_H
#define S0_P0_COMMANDS_H
#define MAX 2048

typedef struct tItem{
    char command[MAX];
    void (*cmd)(char**);
    char help[MAX];
} tItem;

int BreakLine(char *lin,char *pz[]);
void DoCommand(char *pcs[]);
//Prints the names and logins of the program authors. Authors-l prints only the logins and Authors-n prints only the names
void authors (char**);
void pid(char**);
void ppid(char**);
void cd(char**);
void date (char **c);
void historic(char**, HLIST**);
void Open(char**);
void Close(char**);
void Dup(char**);
void infosys(char**);
void help (char **);
void quit();
void Exit();
void bye();

#endif //S0_P0_COMMANDS_H
