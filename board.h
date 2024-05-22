#ifndef BOARD_H
#define BOARD_H

#include "struct.h"
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

//Permet de passer tous les args sous un même void* pour les threads


typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Item root;
    int num_simulations;
} ThreadData;


int IsValidPosition(Item itm, int x, int y);
void UpdateBoard(Item itm, int x, int y);
int CheckCapture(Item itm, int x, int y, int visited[BOARD_SIZE][BOARD_SIZE]);
void RemoveStones(Item itm, int visited[BOARD_SIZE][BOARD_SIZE]);
int IsGameOver(Item itm);
void backpropagate(Item itm, float result);
Item select_node(Item root);
void expand_node(Item itm);
void* thread_func(void *arg);
void flood_fill(char **board, int x, int y, char color, Point *territory, int *size);
float calculate_scores(char **board);
char determine_territory_owner(char **board, Point *territory, int size);
void run_mcts(Item root, int num_threads, int num_simulations);
Item find_best_move(Item root);
void print_children_heuristics(Item parent);
void IA_computing(char** tab, int* x, int* y );
#endif // BOARD_H
