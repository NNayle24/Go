#include "item.h"
// #include "list.c"

// Créé un item, alloue le tableau et la liste enfant
Item createItem(){
    Item node = malloc(sizeof(struct item_)) ;
    node->board = malloc(BOARD_SIZE*sizeof(char *)) ;
    for(int i=0; i<BOARD_SIZE; i++){
        node->board[i] = calloc(BOARD_SIZE, sizeof(char)) ;
    }
    node->parent = NULL;
    node->next = NULL;
    node->prev = NULL;
    node->f = 0.0;
    node->depth = 0;
    node->turn = 0;
    node->child = createList() ;

    return node ;
}


// Free l'item
void freeItem(Item node){
    if(node){
        if(node->board){
            for(int i=0; i<BOARD_SIZE; i++){
                free(node->board[i]) ;
            }
            free(node->board) ; 
        }
        free(node) ;

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
        baby->turn = (father->turn+1)%2 ;
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
