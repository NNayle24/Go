#include <stdio.h>
#include <stdlib.h>

#ifndef ITEM_H
#define ITEM_H

#include "list.h"

#define WH_BOARD 9



// struct list_ ; 

typedef struct item_
{
    struct item_ * parent ;
    struct list_ * child ; 
    struct item_ * next ,  * prev ;
    float f ;
    int depth ; 
    char ** board ;
    int turn ;
} * Item ; 

Item createItem();
void freeItem(Item);
void addParentItem(Item,Item);
void addChildItem(Item,Item);
void addBoard(Item,char**);
void addHeuristic(Item,float);
void addNext(Item,Item);
void addPrev(Item,Item);
void printBoard(Item);

#endif // ITEM_H