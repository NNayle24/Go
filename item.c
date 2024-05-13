#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>

#include "list.h"
#include "item.h"
#include "list.c"

// Create an Item and allocate Board
Item createItem(){
    Item node = malloc(sizeof(struct item_)) ;
    node->board = malloc(WH_BOARD*sizeof(char *)) ;
    for(int i=0; i<WH_BOARD; i++){
        node->board[i] = calloc(WH_BOARD, sizeof(char)) ;
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

void freeItem(Item node){
    if(node){
        if(node->board){
            for(int i=0; i<WH_BOARD; i++){
                free(node->board[i]) ;
            }
            free(node->board) ; 
        }
        free(node) ;

    }

}

void addParentItem(Item father, Item baby){
    baby->parent = father ;
}

void addChildItem(Item father, Item baby){
    if(father && baby) {
        addLast(father->child, baby) ;

        baby->depth = father->depth+1 ;
        baby->parent = father ;
        baby->turn = (father->turn+1)%2 ;
    }

}


void addBoard(Item node, char **board){
    for(int i=0; i<WH_BOARD; i++){
        for(int j=0; j<WH_BOARD; j++){
            node->board[i][j] = board[i][j] ;
        }
    }
}

void addHeuristic(Item node, float value){
    node->f = value ;

}

void addNext(Item prev_n, Item next_n){
    prev_n->next = next_n ;
}


void addPrev(Item prev_n, Item next_n){
    next_n->prev = prev_n ;
}



void printBoard(Item node){
    if(node != NULL){
        for(int i=0; i<WH_BOARD; i++){
            for(int j=0; j<WH_BOARD; j++){
                printf(" %d |", node->board[i][j]) ;
            }
            printf("\n") ;
            for(int j=0; j<WH_BOARD; j++){
                printf("----") ;
            }
            printf("\n") ;
        }
    }
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
    char** board = malloc(WH_BOARD * sizeof(char*));
    for (int i = 0; i < WH_BOARD; i++) {
        board[i] = calloc(WH_BOARD, sizeof(char));
    }
    // Initialisez le plateau avec des valeurs
    addBoard(boardItem, board);
    // Assurez-vous que les valeurs du plateau correspondent
    // à celles que vous avez ajoutées
    // Puis libérez la mémoire du plateau
    for (int i = 0; i < WH_BOARD; i++) {
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