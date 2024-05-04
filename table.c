#include "table.h"

Zobrist init_zobrist(void)
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
}

HT initTable(void)
{
    HT tmp = malloc(sizeof(HT));
    tmp->itms=malloc(sizeof(Item)*HT_LENGHT);
    tmp->count =  0
    int i ; 
    for (i=0;i<HT_LENGHT;i++)
    {
        tmp->itms[i]=NULL;
    }
    return tmp ;
}

void freeTable(HT table)
{
    free(table);
}
void add(HT table ,Item itm)
{
    table->itms[hash(itm)]=itm;
}

Item searchitem(Table,Item)
{
    return (table->itms[hash(itm)] != NULL);
}

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
    return hash ;
}


/*calculate_hash(char board[ROWS][COLS], ZobristKeys *keys) {
    unsigned long long hash = 0; // Initialize hash value to 0

    // Iterate over each cell in the board
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            // XOR the Zobrist key for the character in the current cell with the current hash value
            hash ^= keys->keys[board[i][j]][i][j];
        }
    }

    return hash; // Return the final hash value
}*/