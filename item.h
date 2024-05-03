
struct list_ ; 

typedef struct item_
{
    struct item_ * parent ;
    struct list_ * child ; 
    struct item_ * next ,  * prev ;
    float f ;
    int depth ; 
    char ** board ;
    
} * Item ; 

Item createItem();
void freeItem(Item);
void addParentItem(Item,Item);
void addChildItem(Item,Item);
void addBoard(Item,char**);
void addHeuristic(Item,float);
void addNext(Item,Item);
void addPrev(Item,Item);

