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
void addLast(List);
void addFirst(List);
Item popLast(List);
Item popFirst(List);
Item popBest(List);
short len(Item);