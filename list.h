#ifndef LIST_H
#define LIST_H

//Déclaration avancée pour eviter les soucis d'inclusion circulaire
struct item_;
typedef struct item_ *Item;

#include "item.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct list_ {
    Item first;
    Item last;
    short len;
} *List;

List createList(void);
void freeList(List);
void eraseList(List);
void addLast(List, Item);
void addFirst(List, Item);
Item popLast(List);
Item popFirst(List);
Item popBest(List);
short len(List);
void printList(List);

#ifdef __cplusplus
}
#endif

#endif // LIST_H
