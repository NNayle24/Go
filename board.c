#include "struct.h"

int IsValidPosition(Item itm,int x , int y )
{
    return (itm->board[x][y]==0);
} 

void UpdateBoard(Item itm , int x , int y)
{

}

void GetChildBoard(Item itm , int x ,int y)
{
    Item tmp = createItem();
    addParent(tmp,itm);
    addChildItem(itm,tmp); // add last
    addBoard(tmp,itm->board);
    if (tmp->turn)
    {
        tmp->board[x][y]=-1
    }
    else 
    {
        tmp->board[x][y]=1
    }
    UpdateBoard(itm,int x , int y ) 
} 

int IsFinish(Item itm)
{
    return 1 ;
}



void Compute_Game(Item itm)
{
    if IsFinish(itm)
    {
        addHeuristic(itm)
    }
    else
    {
        int cond = 1 ;
        int  x , y ;
        while (cond)
        {
            x=rand()/BOARD_SIZE;
            y=rand()/BOARD_SIZE;
            if (IsValidPosition(itm,x,y))
            {
                cond = 0 ;
                GetChildBoard(itm,x,y);
                Compute_Game(itm->child->last);
            }
        }
    }
}