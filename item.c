#include "item.h"
// #include "list.c"

// Créé un item, alloue le tableau et la liste enfant
Item createItem() {
    Item node = (Item)malloc(sizeof(struct item_)); 
    if (!node) {fprintf(stderr, "Memory allocation failed for node\n"); return NULL;}

    node->board = (char **)malloc(BOARD_SIZE * sizeof(char *));
    if (!node->board) {
        fprintf(stderr, "Memory allocation failed for node's board\n");
        free(node); // Free node before returning NULL.
        return NULL;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        node->board[i] = (char *)calloc(BOARD_SIZE, sizeof(char));
        if (!node->board[i]) {
            fprintf(stderr, "Memory allocation failed for node's board[%d]\n", i);
            // Free all
            for (int j = 0; j < i; j++) {
                free(node->board[j]); // Free rows
            }
            free(node->board); // Free array
            free(node); // Free itself
            return NULL;
        }
    }

    node->parent = NULL;
    node->next = NULL;
    node->prev = NULL;
    node->f = -1.0;
    node->depth = 0;
    node->turn = 1;
    pthread_mutex_init(&node->lock, NULL);
    node->visits = 0; 
    node->child = createList() ;

    return node ;
}

void freeItem(Item item) {
    if (item == NULL) return;
    //printf("parent:%ld child:%ld board:%ld lock:%ld \n",item->parent,item->child,item->board,item->lock);
    // Libérer le tableau de board
    int i ;
    for (i=0;i<BOARD_SIZE;i++)
    {
        free(item->board[i]);
    }
    free(item->board);


    // Libérer l'Item lui-même
    free(item);

}

// Function to recursively free an Item and all its children
void freeItemTree(Item item) {
    //printf("nombre de fils : %d\n",item->child->len);
    if(item->child->len ==0)
    {
        freeList(item->child);
        freeItem(item);
    }
    else
    {
        // Lock the item to ensure thread safety
        pthread_mutex_lock(&(item->lock));

        // Recursively free all children
        List children = item->child;
        children->len --;
        if (children->len != 0) {
            Item currentChild = children->first;
            while (currentChild != NULL) {
                Item nextChild = currentChild->next;
                //printf("%d ",i++);
                freeItemTree(currentChild);
                currentChild = nextChild;
                children->len --;
            }
        }

        // Unlock the item before destroying the mutex
        pthread_mutex_unlock(&(item->lock));
        pthread_mutex_destroy(&(item->lock));
        freeItem(item);
    }
}



// Ajoute un enfant à l'Item parent
void addParentItem(Item father, Item baby){
    baby->parent = father ;
}


// Ajoute un enfant à la liste enfant du parent
void addChildItem(Item father, Item baby){
    if(father && baby) {
        // Ajoute l'enfant
        addLast(father->child, baby) ;

        // Définie les paramètre de l'enfant
        baby->depth = father->depth+1 ;
        baby->parent = father ;
        baby->turn = -baby->turn ;
    }

}

// Copie un plateau sur le plateau d'un Item
void addBoard(Item node, char **board){
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            node->board[i][j] = board[i][j] ;
        }
    }
}


// Modifie l'heuristique d'un Item
void addHeuristic(Item node, float value){
    node->f = value ;

}

// Ajoute un suivant à un Item
void addNext(Item prev_n, Item next_n){
    prev_n->next = next_n ;
}

// Ajoute un précédent à un Item
void addPrev(Item prev_n, Item next_n){
    next_n->prev = prev_n ;
}


// Affiche le plateau d'un Item
void printBoard(Item node){
    if(node != NULL){
        for(int i=0; i<BOARD_SIZE; i++){
            for(int j=0; j<BOARD_SIZE; j++){
                if(node->board[i][j]==0) printf("   |") ;
                else if(node->board[i][j]==1) printf(" + |") ;
                else printf(" - |") ;
            }
            printf("\n") ;
            for(int j=0; j<BOARD_SIZE; j++){
                printf("----") ;
            }
            printf("\n") ;
        }
    }
    printf("\n") ;
}


/*
int main() {
    // Test de création et libération d'un item
    Item item1 = createItem();
    assert(item1 != NULL);
    freeItem(item1);

    // Test d'ajout de parent
    Item parentItem = createItem();
    Item childItem = createItem();
    addParentItem(parentItem, childItem);
    assert(childItem->parent == parentItem);
    freeItem(parentItem);
    freeItem(childItem);

    // Test d'ajout d'enfant
    Item fatherItem = createItem();
    Item babyItem = createItem();
    addChildItem(fatherItem, babyItem);
    assert(babyItem->parent == fatherItem);
    freeItem(fatherItem);
    freeItem(babyItem);

    // Test d'ajout de plateau
    Item boardItem = createItem();
    char** board = malloc(BOARD_SIZE * sizeof(char*));
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = calloc(BOARD_SIZE, sizeof(char));
    }
    // Initialisez le plateau avec des valeurs
    addBoard(boardItem, board);
    // Assurez-vous que les valeurs du plateau correspondent
    // à celles que vous avez ajoutées
    // Puis libérez la mémoire du plateau
    for (int i = 0; i < BOARD_SIZE; i++) {
        free(board[i]);
    }
    free(board);
    freeItem(boardItem);

    // Test d'ajout de heuristique
    Item heuristicItem = createItem();
    float heuristicValue = 10.5; // Valeur de l'heuristique à tester
    addHeuristic(heuristicItem, heuristicValue);
    assert(heuristicItem->f == heuristicValue);
    freeItem(heuristicItem);

    // Test d'ajout de l'élément suivant
    Item prevItem = createItem();
    Item nextItem = createItem();
    addNext(prevItem, nextItem);
    assert(prevItem->next == nextItem);
    freeItem(prevItem);
    freeItem(nextItem);

    // Test d'ajout de l'élément précédent
    Item previousItem = createItem();
    Item newItem = createItem();
    addPrev(previousItem, newItem);
    assert(newItem->prev == previousItem);
    freeItem(previousItem);
    freeItem(newItem);

    // Test d'impression du plateau
    Item printedItem = createItem();
    // Ajoutez des valeurs à la carte imprimée
    // pour tester la fonction printBoard
    printBoard(printedItem);
    freeItem(printedItem);

    printf("Tous les tests passés avec succès !\n");

    return 0;
}
*/
