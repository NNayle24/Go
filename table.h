#include "item.h"
#include <stdlib.h>
typedef struct Zobrist_
{
    int *** tab;
    int turn ;
} * Zobrist ;

typedef struct HT_
{
    Item * itms;
    int count ;
} * HT ;


Zobrist init_zobrist(void);

HT initTable(void);
void freeTable(HT);
void freeZobrist(Zobrist);
void add(HT,Item);
Item searchitem(HT,Item,Zobrist);
int hash(Item);
