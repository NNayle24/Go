#ifndef LIST_H
#define LIST_H

#include "item.h"
 


typedef struct list_ 
{
    Item first;
    Item last;
    short len ;

} * List ;

List createList(void);
void freeList(List);
void eraseList(List);
void addLast(List, Item);
void addFirst(List, Item);
Item popLast(List);
Item popFirst(List);
Item popBest(List);
short len(List);
void prinList(List) ;

#endif // LIST_H
