#ifndef S0_P0_LIST_H
#define S0_P0_LIST_H
#define MAX 2048

struct HISTORICLIST{
    char *command[MAX];
    int counter;
};


typedef struct HISTORICLIST HLIST;

int IsHistoricEmpty(HLIST*);
void InitHistoric (HLIST*);
int AddHistoricElement(HLIST*, char *);
void ClearHistoric (HLIST*);
void PrintHistoric (HLIST*);
void PrintHistoricLast(HLIST *,int);
char* GetHistoricElement(HLIST *l,int n);
int GetLength(HLIST *L);

#endif //S0_P0_LIST_H
