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
#include <sys/shm.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>


HLIST L;
tfilelist files;
tmemlist memlist;

typedef int key_t;
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
void makedir (char *[]);
void listdir (char *[]);
void erase (char *[]);
void listfile (char *[]);
void reclist(char *[]);
void revlist(char *[]);
void delrec(char *[]);
void allocate (char *[]);
void deallocate (char *[]);
void memfill (char *[]);
void memdump (char*[]);
void readfile(char*[]);
void memory(char*[]);
void Read (char*[]);
void writefile(char*[]);
void Write(char*[]);
void recurse (char*[]);

tItem commands[34] =  {
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
        {"makedir",makedir,"creates a directory"},
        {"erase", erase, "deletes files and/or empty directories"},
        {"reclist",reclist,"lists directories recursively(subdirectories after)"},
        {"revlist",revlist,"lists directories recursively(subdirectories before)"},
        {"listfile", listfile, "gives information about a given file \n listflie [-long][-acc][-link} name1 name2 .. \n -long: gives the long listing of the file\n -acc: accesstime \nlink: if the link is symbolic, the path given"},
        {"listdir", listdir, "lists the contents of the directories"},
        {"delrec",delrec,"deletes directories recursively\n delrec name1 name2 ..."},
        {"allocate", allocate, "allocate [-malloc|-shared|-createshared|-mmap]... Asigna un bloque de memoria\n"
                               "\t-malloc tam: asigna un bloque malloc de tamano tam\n"
                               "\t-createshared cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam\n"
                               "\t-shared cl: asigna el bloque de memoria compartida (ya existente) de clave cl\n"
                               "\t-mmap fich perm: mapea el fichero fich, perm son los permisos"},
        {"deallocate", deallocate, "deallocate [-malloc|-shared|-delkey|-mmap|addr]..	Desasigna un bloque de memoria\n-malloc tam: desasigna el bloque malloc de tamano tam\n"
                                   "\t-shared cl: desasigna (desmapea) el bloque de memoria compartida de clave cl\n"
                                   "\t-delkey cl: elimina del sistema (sin desmapear) la clave de memoria cl\n"
                                   "\t-mmap fich: desmapea el fichero mapeado fich\n"
                                   "\taddr: desasigna el bloque de memoria en la direccion addr\n"},
        {"memfill", memfill, "Fills the memory with character ch, starting at address addr, for cont bytes"},
        {"memdump",memdump,"dumps the contents of cont bytes of memory at address addr to the screen. Dumps hex codes, and for printable characters the associated charrcter"},
        {"readfile",readfile,"Reads cont bytes of a file into memory address addr"},
        {"memory", memory, "[-funcs] Prints the addresses of 3 program functions and 3 library functions"},
        {"read", Read, "Read df addr cont  Reads cont bytes of a file with file descriptor df into memory address addr "},
       {"write",Write,"Write df addr cont writes to a file with descriptor df bytes starting at memory address addr"},
        {"writefile",writefile,"Writefile file addr cont Writes to a file cont bytes start"},
        {"recurse", recurse, "executes the recursive function n times. This funcion has an \n"
                             "automatic array of size 2048. a static array of size 2048 and prints the\n"
                             " addresses of both arrays and its parameter (as well as the number o \n"
                             "recursion) before calling itself"},
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
    printf("Command %s not found\n", pcs[0]);
}


void authors (char **c) {
    if ( c[0]==NULL){
        printf("Jose Martinez Estevez jose.martinez.estevez\nPedro Saavedra Rubinos pedro.saavedra.rubinos\n");
    }
    else{
        if (strcmp(c[0], "-l") == 0)
            printf("jose.martinez.estevez\npedro.saavedra.rubinos\n");
        else if (strcmp(c[0], "-n") == 0)
            printf("Jose Martinez Estevez\nPedro Saavedra Rubinos\n");
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
                printf("%s\n%s\n", commands[i].command, commands[i].help); //help about the command
                found = true;
                break;

            }
        }
        if(!found) printf("No help topics match %s\n", c[0]);
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
                }   }  else printf("Error: historic command not found\n");
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
    FreeFileList(files);
    FreeHistoricList(&L);
    FreeMemList(memlist);
    exit(0);
}

void Exit(char *pcs[]){
    FreeFileList(files);
    FreeHistoricList(&L);
    FreeMemList(memlist);
    exit(0);
}
void Bye(char *pcs[]){
    FreeFileList(files);
    FreeHistoricList(&L);
    FreeMemList(memlist);
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
        printf("Date argument not found\n");
}

void infosys (char *c[]) {
    struct utsname info;
    uname(&info);
    printf("%s\n%s\n%s\n%s\n%s\n", info.sysname,info.machine,info.nodename,info.version,info.release);
}

void cd(char *tr[]) {
    char actualdir[MAX];
    if (tr[0] == NULL && (getcwd(actualdir, MAX) != NULL))
        printf("%s\n", actualdir);
    else {
        if (chdir(tr[0]) != 0) //chdir returns 0 if the directory change was successful and -1 if not
            printf("Not executed: No such file or directory\n");
    }
}

void cwd(char *tr[]) {
    char actualdir[MAX];
    if (tr[0] == NULL && (getcwd(actualdir, MAX) != NULL))
        printf("%s\n", actualdir);
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

char * strmode (mode_t m){
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
                printf("Error opening, descriptor not included\n");
                return;
            }
        if ((df = open(tr[0], mode, 0777)) == -1)
            perror("Cannot open file\n");
        else {
            if (addfile(tr[0], df, mode, &files)) {
                printf("Opened an entry to the list of opened files %s %d %d(%s) \n", tr[0], df, mode,
                       strmode(mode));
            } else printf("Couldn't add file\n");
        }
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
        printf("Error, cant close\n");
        return;
    }
    if (close(df) == -1) {
        perror("Cannot close descriptor\n");
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

    if (tr[0] == NULL || (df = atoi(tr[0])) < 0) { //no hay parametro o el descriptor es menor que 0
        printf("Printing open files\n");
        printopenfiles(files);
        return;
    }
    duplicado = dup(df);
    p = findfile(df, files)->data.filename;
    sprintf(aux, "dup %d (%s)\n", df, p);
    if (addfile(p, duplicado, fcntl(duplicado, F_GETFL), &files))
        printf("Added file %s %d\n", aux, duplicado);
    else printf("Error, could not add file\n");
}

int IsDirectory(char *name){
    struct stat s;
    if(lstat(name, &s) == -1) //returns information about the simbolic link of a file
        return 0;

    return S_ISDIR(s.st_mode);
}


int PrintInfoFile(char *filename, char *dirname, int longl, int link, int acc) {
    struct stat fileStat;
    char path[1024] ,linkTarget[1024];
    snprintf(path, sizeof(path), "%s/%s", dirname, filename);
    if (lstat(path, &fileStat) == -1) { //stat gives us information aboutr the file in question
        return -1;
    }
    if(acc) {
        char time[64];
        strftime(time,sizeof(time),"%Y-%m-%d %H:%M:%S  ", localtime(&fileStat.st_atime));
        printf("%s",time);
    }
    if(longl){

        struct passwd *pws = getpwuid(fileStat.st_uid);//search the user database for an entry with a matching uid (user id)
        struct group *grp = getgrgid(fileStat.st_gid);  // search the group database for an entry with a matching gid (group id)
        printf(
                "%ld(%ld) %s %s %s\n",
                fileStat.st_nlink,
                fileStat.st_ino,
                pws != NULL ? pws->pw_name : "*******",
                grp != NULL ? grp->gr_name : "*******",
                strmode(fileStat.st_mode)
        );
    } // hardlinks,inode,user,group and permissions
    printf("%ld\t%s\n", (long)fileStat.st_size, filename);
    if(link && S_ISLNK(fileStat.st_mode)){
        int length = readlink(path,linkTarget,sizeof(linkTarget) -1);
        if(length!=-1){
            linkTarget[length] = '\0';
            printf("link: %s \n",linkTarget);}
    }else printf(" ");

    return 0;
}


void listfile(char *tr[]){
    if(tr[0] == NULL) cwd(tr);
    else{
        int longl,link,acc,i;
        char actualdir[MAX];

        longl = link = acc = 0;
        for(i=0;tr[i][0] == '-';i++){
            if(!strcmp(tr[i],"-long")) longl = 1;
            else if(!strcmp(tr[i],"-acc")) acc = 1;
            else if(!strcmp(tr[i], "-link")) link = 1;
        }
        for(;tr[i] != NULL;i++){
            if(PrintInfoFile(tr[i],getcwd(actualdir,MAX),longl,link,acc) == -1) printf("error");
        }
    }
}

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
    return 0;
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
        if (ListDir(tr[i], ishid, islong, islink, isacc) == -1) {
            printf("Cannot list %s: %s\n", tr[i], strerror(errno));
        }
    }
}

void erase(char *tr[]){
    if(tr[0]==NULL)
        cwd(tr);
    else{
        for(int i=0;tr[i] != NULL;i++){
            if(remove(tr[i]) != 0)
                printf("Could not remove %s\n",tr[i]);
            else    printf("Removed %s\n",tr[i]);
        }
    }
}

void auxrec(char *dir,int hid,int longl, int acc,int link){
    DIR *p;
    struct dirent *d;
    if ((p = opendir(dir)) == NULL)
        printf("error recursiva");
    printf("--------%s--------\n",dir);
    while((d = readdir(p)) != NULL){
        if(!hid && d->d_name[0] == '.') //if the file is not hidden, then continue
            continue;
        if (PrintInfoFile(d->d_name,dir , longl, link, acc) == -1)
            printf("Cannot list %s: %s\n", d->d_name, strerror(errno));
    }
    rewinddir(p);
    while ((d = readdir(p)) != NULL) {
        if (!hid && d->d_name[0] == '.') //to skip hidden files
            continue;
        struct stat fileStat;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, d->d_name);
        if (stat(path, &fileStat) == -1) { //stat gives us information aboutr the file in question
            printf("error: %s\n",strerror(errno));
        }
        if(S_ISDIR(fileStat.st_mode)){//if file is a directory, call inside it
            auxrec(path,hid,longl,acc,link);
        }}

    if (closedir(p) == -1) { //after finishing, the directory should be closed
        printf("Error closing directory %s: %s\n", d->d_name, strerror(errno));
    }
}

void auxrev(char *dir,int hid,int longl, int acc,int link){

    DIR *p;
    struct dirent *d;
    if ((p = opendir(dir)) == NULL)
        printf("error: %s\n",strerror(errno));

    while ((d = readdir(p)) != NULL) {
        if (!hid && d->d_name[0] == '.')
            continue;
        struct stat fileStat;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, d->d_name);
        if (stat(path, &fileStat) == -1) {
            printf("error: %s",strerror(errno));
        }
        if(S_ISDIR(fileStat.st_mode)){//if file is a directory, call inside it
            auxrev(path,hid,longl,acc,link);
        }}
    rewinddir(p);
    printf("--------%s--------\n",dir);
    while((d = readdir(p)) != NULL){
        if(!hid && d->d_name[0] == '.')
            continue;
        if (PrintInfoFile(d->d_name,dir, longl, link, acc) == -1)
            printf("Cannot list %s: %s\n", d->d_name, strerror(errno));
    }
    if (closedir(p) == -1) { //after finishing, the directory should be closed
        printf("Error closing directory %s: %s\n", d->d_name, strerror(errno));
    }
}

void reclist (char *tr[]){
    if(tr[0] == NULL)
        cwd(tr);
    else{//if there are parameters, check arguments
        int i, ishid, islong, isacc, islink;
        ishid = islong = isacc = islink = 0;
        for (i = 0; tr[i][0] == '-'; i++) {
            if (!strcmp(tr[i], "-hid")) { ishid = 1;}
            else if (!strcmp(tr[i], "-long")){islong = 1;}
            else if (!strcmp(tr[i], "-link")){islink = 1;}
            else if (!strcmp(tr[i], "-acc")){isacc = 1;}
        }
        for(;tr[i] != NULL;i++){//check directories given and start reading
            if(!IsDirectory(tr[i]))
                printf("Error: %s is not a directory\n",tr[i]);
            else{
                auxrec(tr[i],ishid,islong,isacc,islink);
            }
        }
    }
}

void revlist (char *tr[]){
    if(tr[0] == NULL) cwd(tr); else{ //if there are parameters, check arguments
        int i, ishid, islong, isacc, islink;
        ishid = islong = isacc = islink = 0;
        for (i = 0; tr[i][0] == '-'; i++) {
            if (!strcmp(tr[i], "-hid")) { ishid = 1;}
            else if (!strcmp(tr[i], "-long")){islong = 1;}
            else if (!strcmp(tr[i], "-link")){islink = 1;}
            else if (!strcmp(tr[i], "-acc")){isacc = 1;}
        }
        for(;tr[i] != NULL;i++){ //check directories given and start reading
            if(!IsDirectory(tr[i])) printf("Error: %s is not a directory\n",tr[i]);
            else{
                auxrev(tr[i],ishid,islong,isacc,islink);
            }
        }
    }
}

void delaux(char *dir){
    DIR *p;
    struct dirent *d;

    if ((p = opendir(dir)) == NULL){
        printf("Error: Trying to delete the current working directory\n");//open directory
        return;}

    while ((d = readdir(p)) != NULL) {
        if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))//to avoid entering unwanted directories
            continue;
        struct stat fileStat;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, d->d_name);
        if (lstat(path, &fileStat) == -1)
            printf("error\n");
        if(S_ISDIR(fileStat.st_mode)){
            delaux(path);//call again on inside directories
        }else {
            if (remove(path) != 0) //delete files
                printf("error deleting\n");
        }
    }
    if (closedir(p) == -1) { //after finishing, the directory should be closed
        printf("Error closing directory %s: %s\n", dir, strerror(errno));
    }
    if(rmdir(dir) != 0)//delete current directory
        printf("error");
}

void delrec(char *tr[]){
    if (tr[0] == NULL)
        printf("Not executed: No such file or directory\n");
    else
        for(int i=0;tr[i] != NULL;i++){
            delaux(tr[i]);//calls on each directory given

        }}

void makefile (char *tr[]) {
    char actualdir[MAX];
    if (tr[0] == NULL && (getcwd(actualdir, MAX) != NULL))
        printf("%s", actualdir);
    else {
        char* name = tr[0];
        int fd = open(name, O_CREAT | O_RDWR, 0666); // all users can read and write
        if (fd == -1)
            perror("Error creating file");
    }
}

void makedir (char *tr[]){
    if(tr[0] == NULL) cwd(tr);
    if(mkdir(tr[0],0755) != 0) // rwx for owner, rx for the rest
        printf("error creating directory");
    else    printf("Directory %s created",tr[0]);
}
//-------------------------------------------------------  P2  ---------------------------------------------------------------------

char* strdate() {
    static char date[20]; // Static buffer to persist after the function ends
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if (strftime(date, sizeof(date), "%b %d %H", &tm) == 0) {
        fprintf(stderr, "Error: Could not format date.\n");
        return NULL;
    }

    return date;
}

void *String_to_ULong(char *s) {
    unsigned long long *p = malloc(sizeof(unsigned long long));
    if (p == NULL) {
        return NULL;
    }
    *p = strtoull(s, NULL, 16);
    return p;
}


void * ObtenerMemoriaShmget (key_t clave, size_t tam)
{
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;
    //char* Date = strdate();
    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL; /*cuando no es crear pasamos de tamano 0*/
    if (clave==IPC_PRIVATE)  /*no nos vale*/
    {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s); /* si no es crear, necesitamos el tamano, que es s.shm_segsz*/
    //addmem(p, s.shm_segsz,Date,"shared","",clave,&memlist);
    return (p);
}


/////
void * MapearFichero (char * fichero, int protection)
{
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;
    char* Date = strdate();

    if (protection&PROT_WRITE)
        modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
        return NULL;
    if ((p = mmap (NULL,s.st_size, protection,map,df,0)) == MAP_FAILED)
        return NULL;
    addmem(p,s.st_size,Date,"mapped",fichero,df,&memlist);
    addfile(fichero, df, modo, &files);
    return p;
}
void do_AllocateMmap(char *arg[])
{
    char *perm;
    void *p;
    int protection=0;

    if ((perm=arg[1])!=NULL && strlen(perm)<4) {
        if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
        if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
        if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
    }
    if ((p=MapearFichero(arg[0],protection))==NULL)
        perror ("Cannot map file\n");
    else
        printf ("file %s mapped in %p\n", arg[0], p);
}

void do_AllocateShared (char *tr[])
{
    key_t cl;
    void *p;

    cl=(key_t)  strtoul(tr[0],NULL,10);
    mPos q = findmemsh(cl,&memlist); //messes up abstraction but saves a search through the memory list

    if( q == NULL){
        printf("Error,key does not exist\n");
        return;
    }

    long size = q->data.size;

    if ((p=ObtenerMemoriaShmget(cl,0))!=NULL && addmem(p,size,strdate(),"shared","",cl,&memlist))
        printf ("Shared memory of key %lu assigned  in %p\n",(unsigned long) cl, p);
    else
        printf ("Cannot assign shared memory key %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void do_AllocateCreateshared(char *tr[]) {

    key_t cl;
    size_t tam;

    if(tr[1] == NULL){
        printmemory(memlist,"shared");
        return;
    }

    cl = (key_t)strtoul(tr[1], NULL, 10);

    if(tr[2] == NULL)
        tam = 128;
    else
        tam = (size_t)strtoul(tr[2], NULL, 10);

    void *p;
    char* date = strdate();
    p = ObtenerMemoriaShmget(cl, tam);

    if (p != NULL) {
        printf("Allocated %zu bytes at address %p with key %u\n", tam, p,cl);
        if (!addmem(p, tam, date, "shared", "", cl, &memlist)) {
            printf("Error: Unable to record memory allocation.\n");
        }
    } else {
        printf("Cannot assign shared memory key %d: %s\n", cl, strerror(errno));
    }
}

void do_DeallocateDelkey (char *args[])
{
    key_t clave;
    int id;
    char *key=args[0];

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("delkey needs a valid key\n");
        return;
    }
    if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: cannot obtain shared memory\n");
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1) {
        perror("shmctl: cannot delete shared memory\n");
        return;
    }
    printf("Dealocated delkey %d", clave);
}

void allocate(char *tr[]) {
    if (tr[0] == NULL) {
        printmemory(memlist, "all");
        return;
    }

    if (tr[1] == NULL) {
        if (strcmp(tr[0], "-malloc") == 0){
            printmemory(memlist,"malloc");
            return;
        }
        else if (strcmp(tr[0], "-createshared") == 0){
            printmemory(memlist,"shared");
            return;
        }
        else if (strcmp(tr[0], "-mmap") == 0){
            printmemory(memlist,"mapped");
            return;
        }
        else if (strcmp(tr[0], "-shared") == 0){
            printmemory(memlist,"shared");
            return;
        }
        else{
            printf("error\n");
            return;
        }
    }
    if (strcmp(tr[0], "-mmap") == 0) {
        do_AllocateMmap(&tr[1]);
        return;
    }

    if (strchr(tr[1], '.') != NULL) {
        printf("Invalid size: floats are not allowed\n");
        return;
    }

    char *endptr;
    long size = strtol(tr[1], &endptr, 10);

    if (*endptr != '\0' || endptr == tr[1]) {
        printf("Invalid size: '%s' is not a number.\n", tr[1]);
        return;
    }
    if (size <= 0) {
        printf("Invalid size: must be a positive number.\n");
        return;
    }
    if (size > INT_MAX) {
        printf("Invalid size: number is too large.\n");
        return;
    }

    if (strcmp(tr[0], "-malloc") == 0) {
        char *address = malloc(size);
        char *date = strdate();
        if (address == NULL) {
            printf("Memory allocation failed.\n");
            return;
        }
        void* adr = (void *) address;
        printf("allocated %ld bytes at %p\n", size, adr);
        if (!addmem(adr, size, date, "malloc", "", 0, &memlist)) {
            fprintf(stderr, "Error: Could not add memory record. Freeing memory.\n");
            free(address);  // Free only if tracking fails
        }}
    else if (strcmp(tr[0], "-createshared") == 0) {
        do_AllocateCreateshared(tr);

    } else if (strcmp(tr[0], "-shared") == 0) {
        do_AllocateShared(&tr[1]);

    }
}

void deallocate(char *tr[]) {
    if (tr[0] == NULL) {
        printmemory(memlist, "all");
        return;
    }
    if(tr[1]==NULL){
        if (strcmp(tr[0], "-malloc") == 0)
            printmemory(memlist,"malloc");
        else if (strcmp(tr[0], "-createshared") == 0)
            printmemory(memlist,"shared");
        else if (strcmp(tr[0], "-mmap") == 0)
            printmemory(memlist,"mapped");
        else if (strcmp(tr[0], "-shared") == 0)
            printmemory(memlist,"shared");
        else  {
            void* adr = (void*) strtoull(tr[0],NULL,16);
            mPos p = findmemad(adr,&memlist);
            if(p != NULL) {
                closemem(p, &memlist);
                printf("deleting %s\n", tr[0]);
            }
            else printf("error deallocating %s\n",tr[0]);
        }
        return;
    }

    if (strcmp(tr[0], "-malloc")==0) {
        mPos p = findmemsz(atoi(tr[1]), &memlist);
        if (p == NULL) {
            printf("Error\n");
            return;
        } else closemem(p, &memlist);
    }
    else if (strcmp(tr[0], "-mmap")==0){
        mPos p = findmemfl(tr[1],&memlist);
        if (p == NULL)
        {
            printf("Error\n");
            return;
        }
        else closemem(p,&memlist);
    }
    else if (strcmp(tr[0], "-shared")==0){
        int key = atoi(tr[1]);
        mPos p = findmemsh(key,&memlist);
        if (p == NULL)
        {
            printf("Error\n");
            return;
        }
        else closemem(p,&memlist);
    }
    else if (strcmp(tr[0], "-delkey")==0) {
        do_DeallocateDelkey(&tr[1]);
    }
}

void FillMemory (void *adr, size_t cont, unsigned char byte)
{
    unsigned char *arr=(unsigned char *) adr;
    size_t i;

    for (i=0; i<cont;i++)
        arr[i]=byte;
}


void memfill(char* tr[]) {
    if(tr[0] == NULL){
        printmemory(memlist,"all");
        return;
    }
    void* adr = (void*) strtoull(tr[0],NULL,16);

    // sscanf(tr[0],"%p".&adr);
    size_t size = 128;
    unsigned char cont = 'a';


    if(tr[1] != NULL) {
        size =(size_t) strtoull(tr[1],NULL,10);
        if (tr[2] != NULL) {

            cont = tr[2][0] == '"' ?  tr[2][1] != '\0' ? tr[2][1] : '"' : tr[2][0];
        }
    }
    printf("Filling %zu bytes of memory with %02X (%c) at adress %p\n",size,cont,cont,adr);
    FillMemory(adr,size,cont);
}

void memdump(char *tr[]) {
    if(tr[0] == NULL){
        printmemory(memlist,"all");
        return;
    }

    void *adr = (void *) strtoull(tr[0],NULL,16);
    int size = tr[1] != NULL ? atoi(tr[1]) : 128;
    unsigned char *ptr = (unsigned char *)adr;
    printf("Dumping %u bytes from address %p\n",size,adr);
    for (int i = 0; i < size; i++) {
        printf("%02X ", ptr[i]);
        if((i+1) % 16 == 0 || i == size -1){
            printf("\n");
            for(int j = i - (i%16);j <= i;j++){
                printf("%c  ", isprint(ptr[j]) ? ptr[j] : ' ');
            }
            printf("\n");
        }
    }
}


ssize_t LeerFichero (char *f, void *p, size_t cont)
{
    struct stat s;
    ssize_t  n;
    int df,aux;

    if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
        return -1;
    if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont=s.st_size;
    if ((n=read(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

void readfile (char *tr[])
{
    void *p;
    size_t cont=-1;  /*si no pasamos tamano se lee entero */
    ssize_t n;
    if (tr[0]==NULL || tr[1]==NULL){
        printf ("No parameters\n");
        return;
    }

    p = (void*) (uintptr_t) tr[1];
    if (tr[2]!=NULL)
        cont=(size_t) atoll(tr[2]);

    if ((n=LeerFichero(tr[0],p,cont))==-1)
        perror ("Cannot read file");
    else
        printf ("read %lld bytes of %s in %p\n",(long long) n,tr[0],p);
}

void Read(char *tr[]) {
    void *p;
    size_t cont = -1;
    ssize_t n;
    int fd;
    if (tr[0] == NULL || tr[1] == NULL) {
        printf("Invalid arguments\n");
        return;
    }
    fd = atoi(tr[0]);
    if (fd <= 0) {
        printf("Invalid file descriptor: %s\n", tr[0]);
        return;
    }
    p = (void *)(uintptr_t)strtoull(tr[1], NULL, 16);
    if (p == NULL) {
        printf("Invalid buffer address: %s\n", tr[1]);
        return;
    }
    if (tr[2] != NULL) {
        cont = (size_t) atoll(tr[2]);
        if (cont <= 0) {
            printf("Invalid size: %s is not a valid number.\n", tr[2]);
            return;
        }
    }
    else cont = 128;
    if ((n = read(fd, p, cont)) == -1) {
        perror("Cannot read from file descriptor");
    } else {
        printf("Read %lld bytes into %p\n", (long long)n, p);
    }
}

ssize_t EscribirFichero(char *f, void *p, size_t cont) {
    int df;
    ssize_t n;
    df = open(f, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (df == -1) {
        return -1;
    }
    n = write(df, p, cont);
    if (n == -1) {
        close(df);
        return -1;
    }
    close(df);
    return n; //number of bytes written
}

void writefile(char *tr[]) {
    void *p;
    size_t cont = -1;
    ssize_t n;

    if (tr[0] == NULL || tr[1] == NULL) {
        printf("No parameters\n");
        return;
    }
    p = (void *) (uintptr_t) strtoull(tr[1], NULL, 16);
    if (tr[2] != NULL)
        cont = (size_t) atoll(tr[2]);

    n = EscribirFichero(tr[0], p, cont);
    if (n == -1) {
        perror("Cannot write to file");
    } else {
        printf("Written %lld bytes to %s from %p\n", (long long)n, tr[0], p);
    }
}

void Write(char *tr[]){
    void *p;
    size_t cont = -1;
    ssize_t n;
    int fd;
    if (tr[0] == NULL || tr[1] == NULL || tr[2] == NULL) {
        printf("Invalid arguments\n");
        return;
    }
    fd = atoi(tr[0]);
    if (fd <= 0) {
        printf("Invalid file descriptor: %s\n", tr[0]);
        return;
    }
    p = (void *)(uintptr_t)strtoull(tr[1], NULL, 16);
    if (p == NULL) {
        printf("Invalid buffer address: %s\n", tr[1]);
        return;
    }
    if (tr[2] != NULL) {
        cont = (size_t) atoll(tr[2]);
        if (cont <= 0) {
            printf("Invalid size: %s is not a valid number.\n", tr[2]);
            return;
        }
    }
    if ((n = write(fd, p, cont)) == -1) {
        perror("Cannot Write from file descriptor");
    } else {
        printf("Written %lld bytes from %p in file %d\n", (long long)n, p,fd);
    }
}

void print_funcs() {
    printf("Program functions:\t%p\t%p\t%p"
           "\nLibrary functions:\t%p\t%p\t%p\n", help, cd, authors, printf, malloc, free);

}

int ext_var1, ext_var2, ext_var3;
int ext_init_var1 = 10, ext_init_var2 = 20, ext_init_var3 = 30;

void print_vars(){
    static int static_var1, static_var2, static_var3;               // Static uninitialized
    static int static_init_var1 = 40, static_init_var2 = 50, static_init_var3 = 60; // Static initialized
    int local_var1 = 100, local_var2 = 200, local_var3 = 300;
    printf("Local variables :\t%p\t%p\t%p\n", &local_var1,&local_var2,&local_var3);
    printf("Global variables:\t%p\t%p\t%p\n", &ext_init_var1,&ext_init_var1,&ext_init_var3);
    printf("Glob. (N.I.) var:\t%p\t%p\t%p\n", &ext_var1,&ext_var2,&ext_var3);
    printf("Static variables:\t%p\t%p\t%p\n", &static_var1,&static_var2,&static_var3);
    printf("Stat. (N.I.) var:\t%p\t%p\t%p\n", &static_init_var1,&static_init_var2,&static_init_var3);

}
void Do_pmap (void){
    pid_t pid;
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");
        exit(1);
    }
    waitpid (pid,NULL,0);
}

void memory (char *tr[]){
    if (strcmp(tr[0], "-funcs") == 0) {
        print_funcs();
    }
    else if (strcmp(tr[0], "-vars") == 0)
         print_vars();
    else if (!strcmp(tr[0],"-pmap"))
        Do_pmap();
    else if (strcmp(tr[0], "-blocks")==0){
        printmemory(memlist, "all");
    }
    else if (strcmp(tr[0], "-all")==0){
        print_vars();
        print_funcs();
        printmemory(memlist, "all");
    }
    else
        printf("Option %s not contemplated\n", tr[0]);

}

void auxrecurse(int n) {
    int array[2048];
    static int static_array[2048];

    printf("Parameter:(%d)%p\t array: %p\t static arr: %p\n", n, (void *)&n, (void *)&array, (void *)&static_array);
    if (n <= 0) {
        return;
    }
    auxrecurse(n - 1);
}

void recurse(char *tr[]) {
    if (tr[0] == NULL) {
        printf("No input detected\n");
        return;
    }
    if (strchr(tr[0], '.') != NULL) {
        printf("Invalid size: floats are not allowed\n");
        return;
    }
    char *endptr;
    long size = strtol(tr[0], &endptr, 10);
    if (*endptr != '\0' || endptr == tr[1]) {
        printf("Invalid size: '%s' is not a number.\n", tr[0]);
        return;
    }
    if (size <= 0) {
        printf("Invalid size: must be a positive number.\n");
        return;
    }
    if (size > INT_MAX) {
        printf("Invalid size: number is too large.\n");
        return;
    }
    int n = atoi(tr[0]);
    auxrecurse(n);
}


//----------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    char line[MAX];
    char *pcs[MAX / 2];
    createfilelist(&files);
    creatememlist(&memlist);
    if (!initfilelist(&files)) {
        printf("Error, could not initialize filelist");
        return 1;
    }
    InitHistoric(&L);
    while (1) {
        printf("#) ");
        fgets(line, MAX, stdin);
        if (-1 == AddHistoricElement(&L, line)) { printf("error"); }
        BreakLine(line, pcs);
        DoCommand(pcs);
    }
    return 0;
}

