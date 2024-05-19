#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include "item.h"

typedef struct Zobrist_ {
    int ***tab;
    int turn;
} *Zobrist;

typedef struct HT_ {
    Item *itms;
    int count;
} *HT;

Zobrist initZobrist(void);
HT initTable(void);
void freeTable(HT);
void freeZobrist(Zobrist);
void addToTable(HT, Item, Zobrist);
Item searchItem(HT, Item, Zobrist);
int hash(Item, Zobrist);

#endif // TABLE_H