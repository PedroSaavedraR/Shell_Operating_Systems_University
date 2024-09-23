#ifndef S0_P0_LIST_H
#define S0_P0_LIST_H
#define MAX 2048

struct HISTORICLIST{
    char *command[MAX];
    int counter;
};

typedef struct HISTORICLIST HLIST;

int IsHistoricEmpty(HLIST *l);
void InitHistoric (HLIST *l);
int AddHistoricElement(HLIST *l, char * cmd);
int ClearHistoric (HLIST *l);
void PrintHistoric (HLIST *l);

#endif //S0_P0_LIST_H
