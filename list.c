#include "list.h"
//#include "item.c"


// Créé une liste vide
List createList(){
    List tmp = (List)malloc(sizeof(struct list_)) ;
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
            freeItem(tmp) ;
            liste->len-- ;
        }
        liste->first = NULL ;
        liste->last = NULL ;
    }
}

// Ajoute un élément à la fin de la liste
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

// Ajoute un élément au début de la liste
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

// Supprime et renvoie le dernier élément de la liste
Item popLast(List liste){
    Item tmp;
    tmp = liste->last ;
    if(liste->len != 0){
        if(liste->len == 1){
            liste->first = NULL ;
            liste->last = NULL ;
        }
        else{
            liste->last = tmp->prev ;
            liste->last->next = NULL ;
        }
        liste->len-- ;
    }

    return tmp ;
}

// Supprime et renvoie le premier élément de la liste
Item popFirst(List liste){
    Item tmp;
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


// Retourne l'Item avec le f maximal de la liste
Item popBest(List liste){
    Item tmp;
    Item best;
    best = liste->first ;
    tmp = best->next ;
    if(liste->len != 0){
        // Recherche sequentiel Item avec f max
        while(tmp != NULL){
            if(best->f < tmp->f){
                best = tmp ;
            }
            tmp = tmp->next ;
        }

        // Suppression de la liste
        if(liste->first == best){
            popFirst(liste) ;
        }
        else if(liste->last == best){
            popLast(liste) ;
        }
        else{
            tmp = best->prev ;
            tmp->next = best->next ;
            tmp->next->prev = tmp ;
        }
    }

    return best ;      
}


// Renvoie la taille de la liste
short len(List liste){
    return liste->len ;
}


// Affiche la liste
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
            // Pour afficher le contenu du board, on suppose que BOARD_SIZE représente la taille du tableau
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
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
    int j = 1 ;
    while(myList->len != 0){
        printf("---iteration : %d---\n", j) ;
        popBest(myList) ;
        printList(myList) ;
        printf("nb element %d\n", myList->len) ;
        j++ ;
    }

    
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
    
    int k = 1 ;
    while(newList->len != 0){
        printf("---iteration : %d---\n", k) ;
        popBest(newList) ;
        printList(newList) ;
        printf("nb element %d\n", myList->len) ;
        k++ ;
    }
    

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
