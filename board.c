#include "board.h"

//Verifie si la position est ni hors du tableau ni occupé
int IsValidPosition(Item itm, int x, int y) {
    return (x >= 0 && y >= 0 && y < BOARD_SIZE && x < BOARD_SIZE && (itm->board[x][y] != -1) && (itm->board[x][y] != 1));
}

//Verifie et met a jour le tableau si la pierre placée a crée des emprisonnement 
void UpdateBoard(Item itm, int x, int y) {
    int oppositeColor = -(itm->board[x][y]);
    static int visited[BOARD_SIZE][BOARD_SIZE] = {{0}};
    int neighbors[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    for (int i = 0; i < 4; i++) {
        int nx = x + neighbors[i][0];
        int ny = y + neighbors[i][1];
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
            if (itm->board[nx][ny] == oppositeColor) {
                memset(visited, 0, sizeof(visited[0][0]) * BOARD_SIZE * BOARD_SIZE);
                if (CheckCapture(itm, nx, ny, visited)) RemoveStones(itm, visited);
            }
        }
    }
    memset(visited, 0, sizeof(visited[0][0]) * BOARD_SIZE * BOARD_SIZE);
}


//Actualise l'état du board en prenant en compte la nouvelle pierre en x,y
void UpdateBoard(Item itm, int x , int y) {
    // Détermination de la couleur opposée
    int oppositeColor = -(itm->board[x][y]);


    //Tableau pour se souvenir des cases visitées
    static int visited[BOARD_SIZE][BOARD_SIZE] = {{0}};

    //Check si la pierre posée est suicidée
    if(CheckCapture(itm, x, y, visited)) RemoveStones(itm, visited);

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

//Condition de fin de la simulation basé sur la completude du tableau 
int IsGameOver(Item itm) {
    int empty = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (itm->board[i][j] == 0) {
                empty++;
            }
        }
    }
    //Valeur arbitraire, actuellement la valeur de l'avantage du second joueur
    return (empty < (KOMI));
}


//Simule une partie complete retourne si l'IA a gagné
float simulate(Item itm) {
    //printf("simu...\n");
    int x , y ;
    while(!IsGameOver(itm))
    {
        do
        {
            x = rand() % BOARD_SIZE;
            y = rand() % BOARD_SIZE;
        } while(!IsValidPosition(itm,x,y));
        itm->board[x][y]=itm->turn;
        UpdateBoard(itm,x,y);
        itm->turn=-itm->turn;
    }
    return calculate_scores(itm->board);
}


//remonte la branche pour mettre a jour le nombre de visites des noeuds et les victoires
void backpropagate(Item itm, float result) {
    //printf("back...\n");
    while (itm!=NULL)
    {
        pthread_mutex_lock(&itm->lock); 
        itm->visits++;
        itm->f += result ;
        pthread_mutex_unlock(&itm->lock);
        itm = itm->parent;
    }
}




//Implemente l'algorithme de recherche UCT 
Item select_node(Item root) {
    //printf("select...\n");
    Item select = root;
    
    while (select->child != NULL && select->child->len > 0) {
        Item best_child = NULL;
        double best_uct_value = -INFINITY;

        Item current = select->child->first;
        while (current != NULL) {
            double exploitation_value = (current->visits > 0) ? (current->f / current->visits) : 0.0;
            double exploration_value = (current->visits > 0) ? sqrt(log((double)select->visits) / current->visits) : INFINITY;
            double uct_value = exploitation_value + EXPLORATION_PARAMETER * exploration_value;

            if (uct_value > best_uct_value) {
                best_uct_value = uct_value;
                best_child = current;
            }
            current = current->next;
        }
        
        if (best_child == NULL) {
            break;
        }
        select = best_child;
    }
    
    return select;
}




//Genere tous les fils possible d'un plateau
void expand_node(Item itm) {
    //printf("expand...\n");
    pthread_mutex_lock(&itm->lock);
    if (itm->child->len == 0)
    {
        for (int i = 0; i < BOARD_SIZE; i++) 
        {
            for (int j = 0; j < BOARD_SIZE; j++) 
            {
                if(IsValidPosition(itm,i,j))
                {
                    Item tmp = createItem();
                    addBoard(tmp,itm->board);
                    tmp->board[i][j]=itm->turn;
                    tmp->x=i;
                    tmp->y=j;
                    UpdateBoard(tmp,i,j);
                    addParentItem(itm,tmp);
                    addChildItem(itm,tmp);
                }
            }
        }
    }
    pthread_mutex_unlock(&itm->lock);
}

// fonction qui implemente l'algorithme MCTS de maniere concurrente
void* thread_func(void *arg) {
    //printf("thread...\n");
    ThreadData *data = (ThreadData*)arg;
    for(int i = 0; i < data->num_simulations; i++)
    {
        Item itm = select_node(data->root);
        expand_node(itm);
        float res = simulate(itm);
        backpropagate(itm,res);
    }
    return NULL;

}

// Fonction de remplissage par diffusion pour marquer un territoire
void flood_fill(char **board, int x, int y, char color, Point *territory, int *size) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || board[x][y] != 0) {
        return;
    }

    board[x][y] = color;
    territory[*size].x = x;
    territory[*size].y = y;
    (*size)++;

    flood_fill(board, x + 1, y, color, territory, size);
    flood_fill(board, x - 1, y, color, territory, size);
    flood_fill(board, x, y + 1, color, territory, size);
    flood_fill(board, x, y - 1, color, territory, size);
}

// Fonction pour déterminer le propriétaire d'un territoire
char determine_territory_owner(char **board, Point *territory, int size) {
    int black_adjacent = 0;
    int white_adjacent = 0;

    for (int i = 0; i < size; i++) {
        int x = territory[i].x;
        int y = territory[i].y;

        if (x > 0 && board[x - 1][y] == -1) black_adjacent = 1;
        if (x < BOARD_SIZE - 1 && board[x + 1][y] == -1) black_adjacent = 1;
        if (y > 0 && board[x][y - 1] == -1) black_adjacent = 1;
        if (y < BOARD_SIZE - 1 && board[x][y + 1] == -1) black_adjacent = 1;

        if (x > 0 && board[x - 1][y] == 1) white_adjacent = 1;
        if (x < BOARD_SIZE - 1 && board[x + 1][y] == 1) white_adjacent = 1;
        if (y > 0 && board[x][y - 1] == 1) white_adjacent = 1;
        if (y < BOARD_SIZE - 1 && board[x][y + 1] == 1) white_adjacent = 1;
    }

    if (black_adjacent && !white_adjacent) {
        return -1;
    } else if (white_adjacent && !black_adjacent) {
        return 1;
    } else {
        return 0;
    }
}

// Fonction pour calculer les scores des joueurs
// 1.0 si blanc gagne 0.0 si blanc perd
float calculate_scores(char **board) {
    //printf("start calculate\n");
    char **temp_board = (char **)malloc(BOARD_SIZE * sizeof(char *));
    for (int i = 0; i < BOARD_SIZE; i++) {
        temp_board[i] = (char *)malloc(BOARD_SIZE * sizeof(char));
        memcpy(temp_board[i], board[i], BOARD_SIZE * sizeof(char)); 
    }


    Point territory[BOARD_SIZE * BOARD_SIZE];
    int size;
    float black_score = 0.0 ;
    float white_score = KOMI ;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (temp_board[i][j] == 0) {
                size = 0;
                flood_fill(temp_board, i, j, 2, territory, &size);
                char owner = determine_territory_owner(board, territory, size);
                if (owner == -1) {
                    black_score += size;
                } else if (owner == 1) {
                    white_score += size;
                }
            } else if (temp_board[i][j] == -1) {
                (black_score)++;
            } else if (temp_board[i][j] == 1) {
                (white_score)++;
            }
        }
    }

    // Libération de la mémoire du plateau temporaire
    for (int i = 0; i < BOARD_SIZE; i++) {
        free(temp_board[i]);
    }
    free(temp_board);

    return  ((white_score - black_score)>0)*1.0;
}

//Lance l'algorithme MCTS
void run_mcts(Item root, int num_threads, int num_simulations) {
    //printf("start mcts\n");
    pthread_t threads[num_threads];
    ThreadData *thread_data[num_threads];
    for (int i = 0; i < num_threads; i++) {
        thread_data[i] = (ThreadData*)malloc(sizeof(ThreadData));
        if (thread_data[i] == NULL) {
            // Handle allocation failure
            perror("Failed to allocate memory for ThreadData");
            exit(EXIT_FAILURE);
        }
        thread_data[i]->root = root;
        thread_data[i]->num_simulations = num_simulations / num_threads;
        pthread_create(&threads[i], NULL, thread_func, thread_data[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        free(thread_data[i]); // Free allocated memory
    }
}



//Retourne le meilleur fils
Item find_best_move(Item root) {
    Item best_child = NULL;
    int max_visits = -1;
    
    Item child = root->child->first;
    while (child != NULL) {
        if (child->visits > max_visits) {
            best_child = child;
            max_visits = child->visits;
        }
        child = child->next;
    }
    
    return best_child;
}

//fonction de debug qui affiche l'heuristique et le nombre de visite de chaque fils 
void print_children_heuristics(Item parent) {
    if (parent == NULL || parent->child == NULL) {
        printf("Aucun enfant à afficher.\n");
        return;
    }
    int i = 0 ;
    Item child = parent->child->first;
    printf("Heuristiques des enfants du nœud actuel :\n");
    while (child != NULL) {
        float heuristic = (child->visits > 0) ? (child->f / child->visits) : 0.0;
        printf("Child at %d - Heuristic: %f, Visits: %d\n", 
                 i++,heuristic, child->visits);
        child = child->next;
    }
}

//Realise tout la reflexion de l'IA et renvoi le meilleur coup 
void IA_computing(char** tab, int* x, int* y )
{
    Item root = createItem();
    addBoard(root,tab);
    int num_threads = 1;
    int num_simulations = 100000;
    run_mcts(root, num_threads, num_simulations);
    //print_children_heuristics(root);
    Item best_move = find_best_move(root);
    *x = best_move->x ;
    *y = best_move->y ;
    freeItem(root);
}


/*
int main (void)
{
    return 1 ;
}

int main() {
    srand(time(NULL));

    // Initialize the board
    Item board =createItem();

    int x, y;
    while (1) {
        printBoard(board);

        printf("Your move (enter x y): ");
        scanf("%d %d", &x, &y);

        if ((x - y)==0)
        {
            if (calculate_scores(board->board))
            {
                printf("Game over! L'ia a gagné\n");
                break;
            }
        }

        board->board[x][y] = 1;
        UpdateBoard(board, x, y);


        printBoard(board);

        IA_computing(board->board, &x, &y);
        printf("AI move: %d %d\n", x, y);

        board->board[x][y] = -1;
        UpdateBoard(board, x, y);

        if (IsGameOver(board)) {
            printf("Game over!\n");
            break;
        }
    }

    printBoard(board);

    freeItem(board);

    return 0;
}
*/
