#include "struct.h"
#include <pthread.h>


int IsValidPosition(Item itm,int x , int y );
void UpdateBoard(Item itm , int x , int y);
void GetChildBoard(Item itm , int x ,int y);
int IsFinish(Item itm);
void launcher(Item itm);
void* Compute_Game(void* itms);
float GetHeuristic(Item itm );


