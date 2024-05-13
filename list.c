#include <stdio.h>
#include <stdlib.h> 

#include "list.h"
#include "item.h"

// #include "item.c"

List createList(){
    List tmp = malloc(sizeof(struct list_)) ;
    tmp->first = NULL ;
    tmp->last = NULL ;
    tmp->len = 0 ;
    return tmp ;
}



// Free la structure et tout les éléments
void freeList(List liste){
    eraseList(liste) ;
    free(liste) ;
} 

// Free seulement les éléments
void eraseList(List liste){
    Item tmp = liste->first ;
    if(liste){
        while(liste->len > 0){
            tmp = liste->first ;
            liste->first = tmp->next ;
            free(tmp) ;
            liste->len-- ;
        }
        liste->first = NULL ;
        liste->last = NULL ;
    }
}

void addLast(List liste, Item node){
    if(liste->len == 0){
        liste->first = node ;
        liste->last = node ;
    }
    else{
        liste->last->next = node ;
        node->prev = liste->last ;
        liste->last = node ;

    }
    node->next = NULL ;
    liste->len++ ;

}


void addFirst(List liste, Item node){
    if(liste && node){
        if(liste->len == 0){
            liste->first = node ;
            liste->last = node ;
        }
        else{
            liste->first->prev = node ;
            node->next = liste->first ;
            liste->first = node ;
        }
        node->prev = NULL ;
        liste->len++ ;
    }


}



Item popLast(List liste){
    Item tmp = createItem() ;
    tmp = liste->last ;
    if(liste->len != 0){
        if(liste->len == 1){
            liste->first = NULL ;
            liste->last = NULL ;
        }
        else{
            liste->last = tmp->prev ;
            liste->last->next = NULL ;
            tmp->prev ;
        }
        liste->len-- ;
    }

    return tmp ;
}


Item popFirst(List liste){
    Item tmp = createItem() ;
    tmp = liste->first ;
    if(liste->len != 0){
        if(liste->len == 1){
            liste->first = NULL ;
            liste->last = NULL ;
        }
        else{
            liste->first = tmp->next ;
            liste->first->prev = NULL ;
            tmp->next = NULL ;
        }
        liste->len-- ;
    }
    return tmp ;
}

/*
Item popBest(List liste){
    Item tmp = createItem() ;
    Item best = createItem() ;
    tmp = liste->first ;
    best = tmp ;
    if(liste){
        while(tmp->next != NULL){
            if(tmp->f < best->f){
                best = tmp ;
            }
        }
    }
    return best ;

}
*/

short len(List liste){
    return liste->len ;
}


void printList(List list) {
    if(list->len == 0 && list->first == NULL && list->last == NULL){
        printf("\nListe vide\n") ;
    }
    else{
        Item current = list->first;
        while (current) {
            // Impression de quelques propriétés de l'item
            printf("f: %.2f, depth: %d, turn: %d\n", current->f, current->depth, current->turn);
            /*
            // Pour afficher le contenu du board, on suppose que WH_BOARD représente la taille du tableau
            for (int i = 0; i < WH_BOARD; i++) {
                for (int j = 0; j < WH_BOARD; j++) {
                    printf("%d ", current->board[i][j]);
                }
                printf("\n");
            }
            */
            // Passer à l'élément suivant
            current = current->next;
        }
    }
}




/*
int main(){
  // Test de création de liste
    List myList = createList();
    assert(myList != NULL);
    assert(myList->len == 0);

    // Test d'ajout d'éléments à la liste
    Item item1 = createItem();
    item1->f = 1 ;
    Item item2 = createItem();
        item2->f = 2 ;
    Item item3 = createItem();
        item3->f = 3 ;

    addLast(myList, item1);
    assert(myList->len == 1);
    assert(myList->first == item1);
    assert(myList->last == item1);

    addLast(myList, item2);
    assert(myList->len == 2);
    assert(myList->first == item1);
    assert(myList->last == item2);

    addFirst(myList, item3);
    assert(myList->len == 3);
    assert(myList->first == item3);
    assert(myList->last == item2);
    printf("nb element %d\n", myList->len) ;

    printf("3 - 1 - 2\n") ;
    printList(myList) ;


    // Test de suppression d'éléments
    Item poppedFirst = popFirst(myList);
    assert(poppedFirst == item3);
    assert(myList->len == 2);
    assert(myList->first == item1);
    printf("\nnb element %d", myList->len) ;
    printf("\n1 - 2\n") ;
    printList(myList) ;


    Item poppedLast = popLast(myList);
    assert(poppedLast == item2);
    assert(myList->len == 1);
    assert(myList->last == item1);
    printf("\nnb element %d", myList->len) ;
    printf("\n1\n") ;
    printList(myList);

    printf("\n --- test free liste --- \n") ;
    // Ajouter 10 éléments à la liste
    List newList = createList() ;
    for (int i = 0; i < 10; i++) {
        Item newItem = createItem(i);
        newItem->f = i ;
        addLast(newList, newItem);
    }

    printList(newList) ;
    printf("\nnb element %d", newList->len) ;

    eraseList(newList) ;
    printList(newList) ;
    printf("\nnb element %d", newList->len) ;



    // Test de destruction de liste
    freeList(myList);
    free(newList) ;
    printf("\nTous les tests ont réussi !\n");

    return 0;
}
*/