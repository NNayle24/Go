#include "table.h"
//initialise les tables d'indexations des coups
Zobrist initZobrist(void)
{
    int i ,j;
    Zobrist tmp = malloc(sizeof(Zobrist));
    tmp->tab=malloc(sizeof(int**)*BOARD_SIZE);
    for(i=0;i<BOARD_SIZE;i++)
    {
        tmp->tab[i]=malloc(sizeof(int*)*BOARD_SIZE);
        for(j=0;j<BOARD_SIZE;j++)
        {
            tmp->tab[i][j]=malloc(2*sizeof(int));
            tmp->tab[i][j][0]=rand();
            tmp->tab[i][j][1]=rand();
        }
    }
    tmp->turn=rand();
    return tmp;
}

//initialise la table de hachage
HT initTable(void)
{
    HT tmp = malloc(sizeof(HT));
    tmp->itms=malloc(sizeof(Item)*HT_LENGTH);
    tmp->count =  0;
    int i ; 
    for (i=0;i<HT_LENGTH;i++)
    {
        tmp->itms[i]=NULL;
    }
    return tmp ;
}
//libere la mémoire de la table de hachage , a voir si il faut free les items
void freeTable(HT table)
{
    free(table);
}

//ajoute a la table le pointeur de itm
void addToTable(HT table ,Item itm,Zobrist zkey)
{
    table->itms[hash(itm,zkey)]=itm;
}

//renvoi l'existence de l'item dans la structure de données
Item searchItem(HT table ,Item itm, Zobrist zkey)
{
    return(table->itms[hash(itm,zkey)]);
}

//renvoi le hash/index du plateu
int hash(Item itm,Zobrist zkey)
{
    int i , j ;
    int hash = 0;
    if (itm->turn)
    {
        hash = zkey->turn;
    }
    for (i=0;i<BOARD_SIZE;i++)
    {
        for (j=0;j<BOARD_SIZE;j++)
        {
            if (itm->board[i][j]== -1 )
            {
                hash ^= zkey->tab[i][j][0];
            }
            else if (itm->board[i][j] == 1)
            {
                hash ^= zkey->tab[i][j][1];
            }
        }
    }
    return hash%HT_LENGTH ;
}
