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
#include <sys/stat.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

HLIST L;
tfilelist files;

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
void cwd (char *[]);
void makefile (char *[]);
void listdir (char *[]);
void erase (char *[]);


tItem commands[20] =  {
        {"authors",authors,"Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names"},
        {"pid", pid, "Prints the pid of the process executing the shell."},
        {"ppid",ppid,"Prints the pid of the shell’s parent process."},
        {"cd", cd, "Changes the current working directory of the shell to dir (using the chdir system call). When invoked without arguments it prints the current working directory (using the getcwd system call"},
        {"cwd", cwd, "Prints current directory"},
        {"date", date, "Prints the current date in the format DD/MM/YYYY and the current time in the format hh:mm:ss"},
        {"historic",historic,"Shows the historic of commands executed by this shell.\n– historic Prints all the commands that have been input with their order number\n– historic N Repeats command number N (from historic list)\n– historic -N Prints only the lastN comands"},
        {"open",Open,"Opens a file and adds it (together with the file descriptor and the opening mode to the list of shell open files. Open without arguments lists the shell open files. For each file it lists its descriptor, the file name and the opening mode.\nopening modes:\n cr: O_CREAT \t	ap: O_APPEND\n	ex: O_EXCL \t	ro: O_RDONLY\n	rw: O_RDWR \t	wo: O_WRONLY\n	tr: O_TRUNC\n"},
        {"close",Close,"Closes the df file descriptor and eliminates the corresponding item from the list"},
        {"dup",Dup,"Duplicates the df file descriptor (using the dup system call, creating the corresponding new entry on the file list"},
        {"infosys",infosys,"Prints information on the machine running the shell (as obtained via the uname system call/library function)"},
        {"help",help," help displays a list of available commands. help cmd gives a brief help on the usage of command cmd"},
        {"quit",Quit,"Ends the shell"},
        {"bye",Bye,"Ends the shell"},
        {"exit",Exit,"Ends the shell"},
        {"makefile", makefile,  "creates a file"},
        {"erase", erase, "deletes files and/or empty directories"},
        //{"listdir", listdir, "lists the contents of the directories"},
 //       {"listdir", listdir, "listdir [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2 ..	lists the contents in the directories \n-hid: includes hidden files\n-recb: recursive (before)\n-reca: recursive (after)\nrest of the parameters as stat"},
        {NULL, NULL, "\0"},
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
    if (c[0]==NULL){
        PrintHistoric(&L);
        return;
    }
    if(abs(atoi(c[0])) < GetLength(&L)){// if the argument is within the historic
        if (c[0][0]=='-')
            PrintHistoricLast(&L,atoi(c[0]+1));
        else {
            n=atoi(c[0]);
            if(n >= 0 && n < GetLength(&L)){
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

 int IsDirectory(char *name){
   struct stat s;
   if(lstat(name, &s) == -1)
     return 0;

   return S_ISDIR(s.st_mode);
   }

char LetraTF (mode_t m)
{
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}
/*las siguientes funciones devuelven los permisos de un fichero en formato rwx----*/
/*a partir del campo st_mode de la estructura stat */
/*las tres son correctas pero usan distintas estrategias de asignaciÃ³n de memoria*/



char * strmode (mode_t m)
{
    static char permisos[12];
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}
/*
int ListDir(char *dirname, int hid, int longl, int link, int acc) {
    DIR *p;
    struct dirent *d;

    if ((p = opendir(dirname)) == NULL)
        return -1;

    while ((d = readdir(p)) != NULL) {
        if (!hid && d->d_name[0] == '.') //to skip hidden files
            continue;

        if (PrintInfoFile(d->d_name, dirname, longl, link, acc) == -1)
            printf("Cannot list %s: %s\n", d->d_name, strerror(errno));
    }
    if (closedir(p) == -1) { //after finishing, the directory should be closed
        printf("Error closing directory %s: %s\n", dirname, strerror(errno));
        return -1;
    }

    return 0;  // Success
}

void listdir(char *tr[]) {
    int i, ishid, islong, isacc, islink;
    ishid = islong = isacc = islink = 0;
    for (i = 0; tr[i] != NULL; i++) {
        if (strcmp(tr[i], "-hid") == 0) {
            ishid = 1; //hidden files
        } else if (strcmp(tr[i], "-long") == 0) {
            islong = 1; //long format
        } else if (strcmp(tr[i], "-link") == 0) {
            islink = 1; //Symbolic Links
        } else if (strcmp(tr[i], "-acc") == 0) {
            isacc = 1; //Access times
        } else {
            break;
        }
    }
    if (tr[i] == NULL) {
        cwd(tr);
        return;
    }
    for (;tr[i]!=NULL;i++) {
        if (ListDir(tr[i], ishid, islong, isacc, islink) == -1) {
            printf("Cannot list %s: %s\n", tr[i], strerror(errno));
        }
    }
}*/

void erase(char *tr[]){
	if(tr[0]==NULL) cwd(tr);
	else{
	for(int i=0;tr[i] != NULL;i++){
	if(remove(tr[i]) != 0)
		printf("Could not remove %s",tr[i]);
	else    printf("Removed %s",tr[i]);
	}
	}}


    void cd(char *tr[]) {
         if (tr[0] == NULL) {
             printf("Not executed: No such file or directory");
         } else {
             if (chdir(tr[0]) == 0) //chdir returns 0 if the directory change was successful and -1 if not
                 printf("%s", tr[0]); //tr[0] is the new actual directory
             else
                 printf("Not executed: No such file or directory");
         }
     }

     void cwd(char *tr[]) {
         char actualdir[MAX];
         if (tr[0] == NULL && (getcwd(actualdir, MAX) != NULL))
             printf("%s", actualdir);
     }

void Open(char *tr[]) {
    int i, mode = 0, df;
    if (tr[0] == NULL) { //no parameter
        printf("No file included, listing open files\n");
        printopenfiles(files);
        return;
    } else {
        for (i = 1; tr[i] != NULL; i++)
            if (!strcmp(tr[i], "cr")) mode |= O_CREAT;
            else if (!strcmp(tr[i], "ex")) mode |= O_EXCL;
            else if (!strcmp(tr[i], "ro")) mode |= O_RDONLY;
            else if (!strcmp(tr[i], "wo")) mode |= O_WRONLY;
            else if (!strcmp(tr[i], "rw")) mode |= O_RDWR;
            else if (!strcmp(tr[i], "ap")) mode |= O_APPEND;
            else if (!strcmp(tr[i], "tr")) mode |= O_TRUNC;
            else {
                printf("error, opening descriptor not included");
                return;
            }
        if ((df = open(tr[0], mode, 0777)) == -1)
            perror("Cannot open file");
        else {
            if (addfile(tr[0], df, mode, &files)) {
                printf("Opened an entry to the list of opened files %s %d %d(%s) ", tr[0], df, mode,
                       strmode(mode));
            } else printf("Couldn't add file");
        }
    }
}


void makefile (char *tr[]) {
    char actualdir[MAX];
    if (tr[0] == NULL && (getcwd(actualdir, MAX) != NULL))
        printf("%s", actualdir);
    else {
        char* name = tr[0];
        int fd = open(name, O_CREAT | O_RDWR, "0666");
        if (fd == -1)
            perror("Error creating file");
    }
}


void Close(char *tr[]) {
         int df;
         if (tr[0] == NULL || (df = atoi(tr[0])) < 0) { /*no hay parametro o el descriptor es menor que 0*/
             printf("Printing open files\n");
             printopenfiles(files);
             return;
         }
         if (atoi(tr[0]) == 0 || atoi(tr[0]) == 1 || atoi(tr[0]) == 2) {
             printf("error, cant delete");
             return;
         }
         if (close(df) == -1) {
             perror("Cannot close descriptor");
         } else {
             tPos p = findfile(df, files);
             if (p != NULL) {
                 closefile(p, &files);
             } else printf("Error, could not close file\n");
         }
     }


     void Dup(char *tr[]) {
         int df, duplicado;
         char aux[2 * MAX], *p;

         if (tr[0] == NULL || (df = atoi(tr[0])) < 0) { //no hay parametro el descriptor es menor que 0
             printf("Printing open files\n");
             printopenfiles(files);
             return;
         }
         duplicado = dup(df);
         p = findfile(df, files)->data.filename;
         sprintf(aux, "dup %d (%s)", df, p);
         if (addfile(p, duplicado, fcntl(duplicado, F_GETFL), &files))
             printf("Added file %s %d", aux, duplicado);
         else printf("Error, could not add file");
     };


//----------------------------------------------------------------------------------------------------------------
     int main(int argc, char *argv[]) {
         char line[MAX];
         char *pcs[MAX / 2];
         createfilelist(&files);
         if (!initfilelist(&files)) {
             printf("error, could not initialize filelist");
             return 1;
         }

         InitHistoric(&L);
         while (1) {
             printf("#) ");
             fgets(line, MAX, stdin);
             if (-1 == AddHistoricElement(&L, line)) { printf("error"); }
             BreakLine(line, pcs);
             DoCommand(pcs);
             printf("\n");
         }
         return 0;
     }

