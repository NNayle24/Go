#include "struct.h"

int IsValidPosition(Item itm,int x , int y )
{
    return (itm->board[x][y]==0);
} 

//Actualise l'état du board en prenant en compte la nouvelle pierre
void UpdateBoard(Item itm, int x , int y)
{
    // Tableau pour se souvenir des cases visitées
    static int visited[BOARD_SIZE][BOARD_SIZE]={{0}};

    if (CheckCapture(itm,x,y,visited)) RemoveStones(itm,visited);
    memset(visited,0,sizeof(visited[0][0])*BOARD_SIZE*BOARD_SIZE);
}

//Fonction booléenne vérifiant la capture d'une zone en comptant ses libertés, 1 si capture et 0 sinon
int CheckCapture(Item itm, int x, int y, int visited[BOARD_SIZE][BOARD_SIZE])
{
    //Les offset pour trouver les positions des voisins
    int dx[] = {-1,0,1,0};
    int dy[] = {0,1,0,-1};
    
    //On mouille la case
    visited[x][y] = 1;


    for (int  i = 0; i < 4; i++)
    {
        //Les positions des voisins
        int nx = x + dx[i];
        int ny = y + dy[i];

        if(nx>=0 && nx<BOARD_SIZE && ny>=0 && ny<BOARD_SIZE)
        {
            //Si liberté trouvée, pas de capture
            if (itm->board[nx][ny]==0) return 0;
        
            //On vérifie les libertés des voisins de même couleur, donc même groupe
            if(visited[nx][ny]==0 && itm->board[nx][ny]==itm->board[x][y])
            {
                //Si un voisin a une liberté, pas de capture
                if(CheckCapture(itm,nx,ny,visited)==0) return 0;
            }
        }
    }
    return 1; //Aucune liberté trouvée, capture
}

void RemoveStones(Item itm, int visited[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if(visited[i][j]==1) itm->board[i][j]=0;
        }
    }
}

void GetChildBoard(Item itm , int x ,int y)
{
    Item tmp = createItem();
    addParent(tmp,itm);
    addChildItem(itm,tmp); // add last
    addBoard(tmp,itm->board);
    if (tmp->turn)
    {
        tmp->board[x][y]=-1;
    }
    else 
    {
        tmp->board[x][y]=1;
    }
    UpdateBoard(itm, x, y);
} 

int IsGameOver(Item itm)
{
    return 1;
}

float FindHeuristic(Item itm)
{
    return 0.0;
}

void Compute_Game(Item itm)
{
    if(IsGameOver(itm))
        addHeuristic(itm,FindHeuristic(itm));
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