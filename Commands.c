#include <time.h>
#include <stdio.h>
#include "Commands.h"
#include "List.h"
#include <unistd.h>
#include <stdlib.h>
//#include <sys/utsname.h>


struct tItem commands[15] ={
        {"authors",authors,"Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prinbuuuts only the names"},
        {"pid", pid, "Prints the pid of the process executing the shell."},
        {"ppid",ppid,"Prints the pid of the shell’s parent process."},
        {"cd", cd, "Changes the current working directory of the shell to dir (using the chdir system call). When invoked without arguments it prints the current working directory (using the getcwd system call"},
        {"date", date, "Prints the current date in the format DD/MM/YYYY and the current time in the format hh:mm:ss"},
        {"historic",historic,"Shows the historic of commands executed by this shell.\n– historic Prints all the commands that have been input with their order number\n– historic N Repeats command number N (from historic list)\n– historic -N Prints only the lastN comands"},
        {"open",Open,"Opens a file and adds it (together with the file descriptor and the opening mode to the list of shell open files. Open without arguments lists the shell open files. For each file it lists its descriptor, the file name and the opening mode."},
        {"close",Close,"Closes the df file descriptor and eliminates the corresponding item from the list"},
        {"dupe",Dup,"Duplicates the df file descriptor (using the dup system call, creating the corresponding new entry on the file list"},
        {"infosys",infosys,"Prints information on the machine running the shell (as obtained via the uname system call/library function)"},
        {"help",help," help displays a list of available commands. help cmd gives a brief help on the usage of command cmd"},
        {"quit",quit,"Ends the shell"},
        {"Exit",Exit,"Ends the shell"},
        {"bye",bye,"Ends the shell"},
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
        if(!strcmp(pcs[0],commands[i].command)){
            (commands[i].cmd(pcs+1));
            return;
        }
        printf("Command %s not found\n", pcs[0]);
    }
}


void authors (char **c) {
    char *s[MAX];
    int b = BreakLine(*c, s);
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

void help (char **c) {
    char *s[MAX];
    int i;
    int b = BreakLine(*c, s);
    if (b == 1) {
        for (i = 0; strcmp(commands[i].help, "\0") != 0; i++) {
            printf("%s\n", commands[i].command);
        }
    }
    if (b == 2) {
        for (i = 0; !strcmp(c[b], commands[i].command); i++) {//searches the function for the command we want help about
            printf("%s\n", commands[i].command);//name of the command
            printf("%s", commands[i].help);//help about the command
        }
    }
}

void historic(char **c, HLIST l) {
    char *s[MAX];
    int i;
    int b = BreakLine(*c, s);
    if (b == 1) {
        if (IsHistoricEmpty(&l))
            return;
        else
            for (i = 0; i <= l.counter; i++) {
                printf("%d %s\n", i, l.command[i]);
            }
    } else if (b == 2) {
        int j = atoi(c[b]);
        DoCommand(&l.command[j]);
        }
    }



void pid(char *tr[]) {
    printf("%d\n", getpid());
}


/*void ppid(char *tr[]){
    printf("%d\n", getppid());
}*/

void quit(){
    exit(0);
}

void Exit(){
    exit(0);
}
void bye(){
    exit(0);
}
/*
void date (char c){
    time_t time_now;
    time_now = localtime()

}

void infosys () {

}
*/

void cd (char *tr[]) {
    char actualdir[MAX];
    if (tr[0] == NULL) {
        if (getcwd(actualdir, MAX) == NULL) //if the directory is not found
            printf("ERROR");
        else
            printf("%s", actualdir);
    } else {
        if (chdir(tr[0]) == 0) //chdir returns 0 if the directory change was successful and -1 if not
            printf("%s", tr[0]); //tr[1] is the new actual directory
        else
            printf("ERROR");
    }
}

void Open (char * tr[])
{
    int i,df, mode=0;

    if (tr[0]==NULL) { /*no hay parametro
        ..............ListarFicherosAbiertos...............
        return;
    }
    for (i=1; tr[i]!=NULL; i++)
        if (!strcmp(tr[i],"cr")) mode|=O_CREAT;
        else if (!strcmp(tr[i],"ex")) mode|=O_EXCL;
        else if (!strcmp(tr[i],"ro")) mode|=O_RDONLY;
        else if (!strcmp(tr[i],"wo")) mode|=O_WRONLY;
        else if (!strcmp(tr[i],"rw")) mode|=O_RDWR;
        else if (!strcmp(tr[i],"ap")) mode|=O_APPEND;
        else if (!strcmp(tr[i],"tr")) mode|=O_TRUNC;
        else break;

    if ((df=open(tr[0],mode,0777))==-1)
        perror ("Imposible abrir fichero");
    else{
        ...........AnadirAFicherosAbiertos (descriptor...modo...nombre....)....
        printf ("Anadida entrada a la tabla ficheros abiertos..................",......);
    }
*/


