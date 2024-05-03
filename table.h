#include "item.h"

typedef struct hash_item {
    char* key;
    Item value;
} * Hitem;

typedef struct hash_table {
    int size;
    int count;
    Hitem * items;
} * Table;

Table createTable(void);
Hitem createHitem(Item);
void freeTable(Table);
void freeHitem(Hitem);
void addHitem(Table,Hitem);
void delHitem(Table,Item);
Item searchHitem(Table,char**);
char hash(char**);