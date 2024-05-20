#include "board.h"

//GCC memset
void* memset (void *dest, int val, size_t len)
{
  unsigned char *ptr = dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}

int IsValidPosition(Item itm, int x, int y) {
    return ( x>=0 && y>=0 && y < BOARD_SIZE && x<BOARD_SIZE && (itm->board[x][y] != -1) && (itm->board[x][y] != 1));
}

//Actualise l'état du board en prenant en compte la nouvelle pierre en x,y
void UpdateBoard(Item itm, int x , int y) {
    // Détermination de la couleur opposée
    int oppositeColor = -(itm->board[x][y]);


    //Tableau pour se souvenir des cases visitées
    static int visited[BOARD_SIZE][BOARD_SIZE] = {{0}};

    //Check si la pierre posée est suicidée
    //if(CheckCapture(itm, x, y, visited)) RemoveStones(itm, visited);

    //Parcourir les voisins pour vérifier les captures
    int neighbors[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    for (int i = 0; i < 4; i++) {
        int nx = x + neighbors[i][0];
        int ny = y + neighbors[i][1];
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {

            //Explorer la possibilité de prise seulement sur les pierres ennemies
            if (itm->board[nx][ny] == oppositeColor) {
                memset(visited, 0, sizeof(visited[0][0]) * BOARD_SIZE * BOARD_SIZE);//Reset du masque des visités

                //Verifier si groupe capturable, si oui retirer les pierres
                if (CheckCapture(itm, nx, ny, visited)) RemoveStones(itm, visited);
            
            }
        }
    }

    //Reset du masque des visités
    memset(visited, 0, sizeof(visited[0][0]) * BOARD_SIZE * BOARD_SIZE);
}


/**/
//Fonction booléenne vérifiant la capture d'une zone en comptant ses libertés, 1 si capture et 0 sinon
int CheckCapture(Item itm, int x, int y, int visited[BOARD_SIZE][BOARD_SIZE])
{
    //Les offset pour trouver les positions des voisins
    int dx[] = {-1,0,1,0};
    int dy[] = {0,1,0,-1};
    
    //On mouille la pierre
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

//Retire les pierres capturées du board
void RemoveStones(Item itm, int visited[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            //Les pierres à retirer sont visitées dans CheckCapture
            if(visited[i][j]==1) itm->board[i][j]=0;
        }
    }
}



void GetChildBoard(Item itm, int x, int y) {
    Item tmp = createItem();
    addParentItem(tmp, itm);
    addChildItem(itm, tmp); // add last
    addBoard(tmp, itm->board);
    if (tmp->turn) {
        tmp->board[x][y] = -1;
    } else {
        tmp->board[x][y] = 1;
    }
    UpdateBoard(itm, x, y);
}

//Vérifie si le board est suffisamment rempli pour le considérer final
int IsGameOver(Item itm) {
    int vides = 0;
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (itm->board[i][j] == 0) {
                vides++;
            }
        }
    }
    //Valeur arbitraire, actuellement la valeur de l'avantage du second joueur
    return (vides>KOMI);
}

//Lance des simulations de parties dans des threads
void runGame(Item itm, HT hashTable, Zobrist zkey) {
    pthread_t threads[MAX_THREADS];
    int i, num_threads = 0;

    //Compilation des arguments pour les threads
    ComputeGameArgs args;
    args.itm = itm;
    args.hashTable = hashTable;
    args.zKey = zkey;

    while (num_threads < MAX_THREADS) {
        pthread_create(&threads[num_threads], NULL, ComputeGame, &args);
        num_threads++;
    }
    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

//Joue une partie jusqu'à sa complétion
void* ComputeGame(void* args) {
    //Décompilations des arguments
    ComputeGameArgs* gameArgs = (ComputeGameArgs*)args;
    Item itm = gameArgs->itm;
    HT HashTable = gameArgs->hashTable;
    Zobrist zkey = gameArgs->zKey;

    if (IsGameOver(itm)) {
        addHeuristic(itm, 0.0); //@TODO replace float by getWinner function
    } 
    else {
        int cond = 1;
        int x, y;
        while (cond) {
            x = rand() % BOARD_SIZE;
            y = rand() % BOARD_SIZE;
            if (IsValidPosition(itm, x, y)) {
                cond = 0;
                GetChildBoard(itm, x, y);
                Item tmp = searchItem(HashTable, itm->child->last, zkey);
                if (tmp != NULL) {
                    freeItem(popLast(itm->child));
                    addParentItem(tmp, itm);
                    addChildItem(itm, tmp); // add last
                    addToTable(HashTable, itm->child->last, zkey);
                }
                
                ComputeGameArgs nextComputeGameArgs;
                nextComputeGameArgs.itm = tmp;
                nextComputeGameArgs.hashTable = HashTable;
                nextComputeGameArgs.zKey = zkey;

                ComputeGame(&nextComputeGameArgs);
            }
        }
    }
    return NULL;
}

float GetHeuristic(Item itm) {
    if (itm->f != -1.0) {
        return itm->f;
    }
    float score = 0;
    int i;
    Item current = itm->child->first;
    for (i = 0; i < itm->child->len; i++) {
        score += GetHeuristic(current);
        current = current->next;
    }
    score = score / itm->child->len;
    addHeuristic(itm, score);
    return score;
}

int move(Item itm, HT hashTable, Zobrist zkey) {
    if (IsGameOver(itm)) {
        return 1;
    }
    runGame(itm, hashTable, zkey);
    GetHeuristic(itm);
    Item max = popBest(itm->child);
    eraseList(itm->child);
    *itm = *max ;
    freeItem(itm -> parent);
    itm->parent = NULL ; 
    return 0;
}
