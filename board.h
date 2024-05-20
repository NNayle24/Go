#ifndef BOARD_H
#define BOARD_H

#include "struct.h"
#include <pthread.h>
#include <string.h>

//Permet de passer tous les args sous un même void* pour les threads
typedef struct ComputeGameArgs_ {
    Item itm;
    HT hashTable;
    Zobrist zKey;
} ComputeGameArgs;

typedef struct {
    int x;
    int y;
} Point;


int IsValidPosition(Item, int, int);
void UpdateBoard(Item, int, int);
int CheckCapture(Item, int, int, int[BOARD_SIZE][BOARD_SIZE]);
void RemoveStones(Item, int[BOARD_SIZE][BOARD_SIZE]);
void GetChildBoard(Item, int, int);
int IsGameOver(Item);
void runGame(Item, HT, Zobrist);
void* ComputeGame(void*);
float GetHeuristic(Item);
void flood_fill(char **board, int x, int y, char color, Point *territory, int *size);
char determine_territory_owner(char **board, Point *territory, int size);
float calculate_scores(char **board);

#endif // BOARD_H
