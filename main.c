/*
José Martínez Estévez     jose.martinez.estevez@udc.es
Pedro Saavedra Rubinos    pedro.saavedra.rubinos@udc.esb
*/
#define MAX 2048
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include "List.h"
#include "files.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <string.h>
#include <fcntl.h>

HLIST L;
tfilelist *files;

typedef struct Item{
    char *command;
    void (*cmd)(char**);
    char *help;
} tItem;

int BreakLine(char *lin,char *pz[]);
void DoCommand(char *pcs[]);
void authors (char*[]);
void pid(char*[]);
void ppid(char*[]);
void cd(char*[]);
void date (char*[]);
void historic(char*[]);
void Open(char*[]);
void Close(char*[]);
void Dup(char*[]);
void infosys(char*[]);
void help (char *[]);
void Quit(char *[]);
void Exit(char *[]);
void Bye(char *[]);

tItem commands[] =  {
        {"authors",authors,"Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names"},
        {"pid", pid, "Prints the pid of the process executing the shell."},
        {"ppid",ppid,"Prints the pid of the shell’s parent process."},
        {"cd", cd, "Changes the current working directory of the shell to dir (using the chdir system call). When invoked without arguments it prints the current working directory (using the getcwd system call"},
        {"date", date, "Prints the current date in the format DD/MM/YYYY and the current time in the format hh:mm:ss"},
        {"historic",historic,"Shows the historic of commands executed by this shell.\n– historic Prints all the commands that have been input with their order number\n– historic N Repeats command number N (from historic list)\n– historic -N Prints only the lastN comands"},
        {"open",Open,"Opens a file and adds it (together with the file descriptor and the opening mode to the list of shell open files. Open without arguments lists the shell open files. For each file it lists its descriptor, the file name and the opening mode."},
        //{"close",Close,"Closes the df file descriptor and eliminates the corresponding item from the list"},
        //{"dup",Dup,"Duplicates the df file descriptor (using the dup system call, creating the corresponding new entry on the file list"},
        {"infosys",infosys,"Prints information on the machine running the shell (as obtained via the uname system call/library function)"},
        {"help",help," help displays a list of available commands. help cmd gives a brief help on the usage of command cmd"},
        {"quit",Quit,"Ends the shell"},
        {"bye",Bye,"Ends the shell"},
        {"exit",Exit,"Ends the shell"},
        {"\0", NULL, "\0"},
};

int BreakLine(char *lin,char *pz[]){
    int i;
    if((pz[0]=strtok(lin," \t\n"))==NULL)
        return 0;
    i=1;
    while((pz[i]=strtok(NULL," \t\n"))!=NULL)
        i++;
    return i;
}

void DoCommand(char *pcs[]){
    int i;
    if(pcs[0] == NULL)
        return;
    for(i=0;commands[i].cmd != NULL;i++){
        if(!strcmp(pcs[0],commands[i].command)) {
            (commands[i].cmd(pcs + 1));
            return;
        }
    }
    printf("Command %s not found", pcs[0]);
}


void authors (char **c) {
    if ( c[0]==NULL){
        printf("Jose Martinez Estevez jose.martinez.estevez\nPedro Saavedra Rubinos pedro.saavedra.rubinos");
    }
    else{
        if (strcmp(c[0], "-l") == 0)
            printf("jose.martinez.estevez\npedro.saavedra.rubinos");
        else if (strcmp(c[0], "-n") == 0)
            printf("Jose Martinez Estevez\nPedro Saavedra Rubinos");
        else printf("error checking auth argument");
    }
}

void help (char **c) {
    int i;
    bool found = false;
    if (c[0] == NULL) {
        for (i = 0; strcmp(commands[i].help, "\0") != 0; i++) {
            printf("%s\n", commands[i].command);
        }
    } else {
        for (i = 0; commands[i].cmd != NULL; i++) { //searches the function for the command we want help about
            if (!strcmp(c[0], commands[i].command)) {//name of the command
                printf("%s\n%s", commands[i].command, commands[i].help); //help about the command
                found = true;
                break;

            }
        }
        if(!found) printf("no help topics match %s", c[0]);
    }

}

void historic(char **c) {
    char *tr[MAX/2];
    int n;
    char p[MAX];
    if (c[0]==NULL){//if there are no arguments
        PrintHistoric(&L);
        return;
    }
    if(abs(atoi(c[0])) < GetLength(&L)){// if the argument is within the historic
        if (c[0][0]=='-')
            PrintHistoricLast(&L,atoi(c[0]+1));
        else {
            n=atoi(c[0]);
            if(n < GetLength(&L)){
                if ((strcpy(p,GetHistoricElement(&L,n))!=NULL)){
                    BreakLine(p, tr);
                    DoCommand(tr);
                }   }  else printf("error, historic command not found");
        }

    }
    else printf("Error, historic not long enough");
}

void pid(char *tr[]) {
    printf("%d", getpid());
}


void ppid(char *tr[]){
    printf("%d\n", getppid());
}

void Quit(char *pcs[]){
    exit(0);
}

void Exit(char *pcs[]){
    exit(0);
}
void Bye(char *pcs[]){
    exit(0);
}

void date(char *c[]) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if (c[0]==NULL)
        printf("%d/%d/%d\n %d:%d:%d",tm.tm_mday, tm.tm_mon + 1,  tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    else if (strcmp(c[0],"-d")==0)
        printf("%d/%d/%d", tm.tm_mday, tm.tm_mon + 1,  tm.tm_year + 1900);
    else if (strcmp(c[0],"-t")==0)
        printf("%d:%d:%d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    else
        printf("Date argument not found");
}
void infosys (char *c[]) {
    struct utsname info;
    uname(&info);
    printf("%s\n%s\n%s\n%s\n%s\n", info.sysname,info.machine,info.nodename,info.version,info.release);

}


void cd (char *tr[]) {
    char actualdir[MAX];
    if (tr[0] == NULL) {
        if (getcwd(actualdir, MAX) == NULL) //if the directory is not found
            printf("ERROR");
        else
            printf("%s", actualdir);
    } else {
        if (chdir(tr[0]) == 0) //chdir returns 0 if the directory change was successful and -1 if not
            printf("%s", tr[0]); //tr[0] is the new actual directory
        else
            printf("ERROR");
    }
}

void Open (char * tr[]) {
    int i, df, mode = 0;

    if (tr[0] == NULL) { //no hay parametro
        printopenfiles(*files);
        return;
    }
    for (i = 1; tr[i] != NULL; i++)
        if (!strcmp(tr[i], "cr")) mode |= O_CREAT;
        else if (!strcmp(tr[i], "ex")) mode |= O_EXCL;
        else if (!strcmp(tr[i], "ro")) mode |= O_RDONLY;
        else if (!strcmp(tr[i], "wo")) mode |= O_WRONLY;
        else if (!strcmp(tr[i], "rw")) mode |= O_RDWR;
        else if (!strcmp(tr[i], "ap")) mode |= O_APPEND;
        else if (!strcmp(tr[i], "tr")) mode |= O_TRUNC;
        else break;

    if ((df = open(tr[0], mode, 0777)) == -1)
        perror("Cannot open file");
    else {
        if(addfile(tr[1],df,mode,files)){
            printf("Anadida entrada a la tabla ficheros abiertos %s %d %d ", tr[i],df,mode);
        }else printf("Couldn't add file");}
}
/*
void Close (char **tr[]){
        int df;

        if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro o el descriptor es menor que 0*//*
            printopenfiles(files);
            return;
        }


        if (close(df)==-1)
            perror("Cannot close descriptor");
        else
        ........EliminarDeFicherosAbiertos......
    }


void Dup (char ** tr[])
{
        int df, duplicado;
        char aux[MAXNAME],*p;

        if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro
            ListOpenFiles(-1);                 /*o el descriptor es menor que 0
            return;
        }

        duplicado=dup(df);
        p=.....NombreFicheroDescriptor(df).......;
        sprintf (aux,"dup %d (%s)",df, p);
        .......AnadirAFicherosAbiertos......duplicado......aux.....fcntl(duplicado,F_GETFL).....;
    }; */
//----------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]){
    char line [MAX];
    char *pcs[MAX/2];
    tfilelist l;
    createfilelist(&l);
    InitHistoric(&L);
    while(1){
        printf("#) ");
        fgets(line, MAX, stdin);
        if(-1 == AddHistoricElement(&L,line)){printf("error");}
        BreakLine(line,pcs);
        DoCommand(pcs);
        printf("\n");
    }
    return 0;
}


