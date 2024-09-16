/*
José Martínez Estévez     jose.martinez.estevez@udc.es
Pedro Saavedra Rubinos    pedro.saavedra.rubinos@udc.esb
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//#include "shell_list.h"

#include <stdio.h>
#include <string.h>
#define MAX 2048


char DoCommand(char *pcs[],tList c,bool terminate){
    int i=0;
    while((pcs[i]=strtok(NULL,"\t\n"))!=NULL)
    {
        if(findItemS(pcs[i],c)== NULL){
            break;
        }else{

        }

        i++;
    }
}

int comread(int argc,char *argv[]){
    char line[MAX];
    char *pcs[MAX/2];
    while(1){
        printf("#)");
        fgets(line,MAX,stdin);
        if(BreakLine(line,pcs)==0)
            continue;
        DoCommand(pcs);
    }
}

void comprocess{

}; */

int main(){
    bool terminate = false;
    while(!terminate){
        comread(0,0);
        //process read comment from command history
    }
    return 0;
}

//--------------------------------------------------------------------------------------

char authors(char input){
    if (pcs[1] != NULL)
        if

}


/*
void Cmd_open (char * tr[])
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

    void Cmd_close (char *tr[])
    {
        int df;

        if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro
            ..............ListarFicherosAbiertos............... /*o el descriptor es menor que 0
            return;
        }


        if (close(df)==-1)
            perror("Inposible cerrar descriptor");
        else
        ........EliminarDeFicherosAbiertos......
    }

    void Cmd_dup (char * tr[])
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
    }

    */