#ifndef ITEM_H
#define ITEM_H


//Déclaration avancée pour eviter les soucis d'inclusion circulaire
struct list_;
typedef struct list_ *List;


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "const.h"
#include "list.h"

typedef struct item_ {
    struct item_ *parent;
    struct list_ *child;
    struct item_ *next, *prev;
    float f;
    int depth;
    char **board;
    int turn;
    pthread_mutex_t lock;
    int visits;
    int x , y ;
} *Item;

Item createItem();
void freeItem(Item);
void addParentItem(Item, Item);
void addChildItem(Item, Item);
void addBoard(Item, char **);
void addHeuristic(Item, float);
void addNext(Item, Item);
void addPrev(Item, Item);
void printBoard(Item);

#ifdef __cplusplus
}
#endif

#endif // ITEM_H
