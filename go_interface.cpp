//Pour compiler avec g++ il faut rajouter les options -lsfml-graphics -lsfml-window -lsfml-system
//Centre des cercles possible probleme car position top left
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#define N 9
#define RADIUS 50
#define COLOR_PLAYER 1

class GoBoard
{
	private:
	sf::VertexArray hlines;
	sf::VertexArray vlines;
	
	public:
	GoBoard(){
		hlines.setPrimitiveType(sf::Lines);
		vlines.setPrimitiveType(sf::Lines);
	}
	void draw(sf::Vector2u c){																// c correspond au dimension de la fenetre de jeu
		int i;																				// c.x est la longueur c.y est la largeur
        int a = 0.85*c.y/N;																	// a équivaut a la taille d'une case
		for (i=0;i<N;i++)
		{
			hlines.append(sf::Vertex(sf::Vector2f(c.x/2-0.85*(c.y/2),0.1*c.y/2+i*a)));		//Pour centrer en x on calcule (size_window_x/2)-(size_table/2)
			hlines.append(sf::Vertex(sf::Vector2f(c.x/2+0.85*(c.y/2)-a,0.1*c.y/2+i*a)));	//Pour centre en y on calcule (size_window_y/2)-(size_table/2)
			vlines.append(sf::Vertex(sf::Vector2f(c.x/2-0.85*(c.y/2)+i*a,0.1*c.y/2)));
			vlines.append(sf::Vertex(sf::Vector2f(c.x/2-0.85*(c.y/2)+i*a,0.1*c.y/2+(N-1)*a)));
		}
	}
	void display(sf::RenderWindow &window)
	{
		window.draw(hlines);
		window.draw(vlines);
	}
};
class BotStones
{
	private:
	sf::CircleShape bot_stone;
	std::vector<float> centersW_x;
	std::vector<float> centersW_y;
	std::vector<float> centersB_x;
	std::vector<float> centersB_y;
	
	public:
	BotStones() : bot_stone(RADIUS){}
	void BoardToVector(sf::Vector2u c,int ** board)							// la fonction transforme notre plateau en un array de vecteur avec les coordonnée
	{																						// c correspond au dimension de la fenetre de jeu board est le plateau	
		int a = 0.85*c.y/N;
		for(int i=0;i<N;i++)
		{
			for(int j=0;j<N;j++)
			{
				if (board[i][j]==1)
				{
					centersW_x.push_back(c.x/2-0.85*(c.y/2)+(i-1)*a+RADIUS);
					centersW_y.push_back(0.1*c.y/2+(j-1)*a+RADIUS);
					
				}
				else if (board[i][j]==-1)
				{
					centersB_x.push_back(c.x/2-0.85*(c.y/2)+(i-1)*a+RADIUS);
					centersB_y.push_back(0.1*c.y/2+(j-1)*a+RADIUS);
				}
			}
		}
	}
	void display(sf::RenderWindow &window)
	{
		bot_stone.setFillColor(sf::Color::White);
		for(int i=0; i< centersW_x.size();i++)
		{
			bot_stone.setPosition(centersW_x[i],centersW_y[i]);
			window.draw(bot_stone);
		}
		bot_stone.setFillColor(sf::Color::Black);
		for(int i=0;i<centersB_x.size();i++)
		{
			bot_stone.setPosition(centersB_x[i],centersB_y[i]);
			window.draw(bot_stone);
		}
		
	}
};

class PlayerStones
{
	private:
	sf::CircleShape player_stone;
	public:
	PlayerStones(){
		player_stone.setRadius(RADIUS);
		if(COLOR_PLAYER==1) player_stone.setFillColor(sf::Color::White);
	}
	
	void display(sf::RenderWindow &window,sf::Vector2i pos,sf::Vector2u f,int ** board)
	{	
		
		int a = 0.85*f.y/N;
		int corner_hg_x,corner_hg_y;													// On définit le coin haut gauche du tableau et on connait celui d'en bas a droite
		corner_hg_x = (f.x/2)-0.85*(f.y/2)-a/2;corner_hg_y = 0.1*(f.y/2)-a/2;			// en ajoutant la taille du tableau en x et en y
		int X,Y;
		X=((pos.x-corner_hg_x)*N)/(a*N);Y=((pos.y-corner_hg_y)*N)/(a*N);
		if ((pos.x>corner_hg_x) && (pos.y>corner_hg_y) && (pos.x<corner_hg_x+a*(N)) && (pos.y<corner_hg_y+a*(N) )){

			player_stone.setPosition(corner_hg_x+(X*a),corner_hg_y+(Y*a));
			if (COLOR_PLAYER==-1 && board[X][Y]==0){
				board[X][Y]=-1;
				window.draw(player_stone);
			}
			else if (COLOR_PLAYER==1 && board[X][Y]==0){
				board[X][Y]=1;
				window.draw(player_stone);
			}
		}

	}


};
int** creerMatriceAleatoire() {
    
    // Allocation dynamique de la mémoire pour la matrice
    int** matrix = new int*[N];
    for (int i = 0; i < N; ++i) {
        matrix[i] = new int[N];
    }

    srand(time(nullptr)); // Initialisation de la graine pour la génération de nombres aléatoires

    // Remplissage de la matrice avec des valeurs aléatoires entre -1, 0 et 1
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int randomValue = rand() % 3; // Génère des valeurs entre 0 et 2
            matrix[i][j] = randomValue == 2 ? -1 : randomValue;
        }
    }

    return matrix; // Retourne un pointeur vers la matrice
}
void afficherMatrice(int** matrix) {

    // Affichage de la matrice
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << matrix[j][i] << " ";
        }
        std::cout << std::endl;
    }
	std::cout<<std::endl;
}

int main()
{
	int ** m =creerMatriceAleatoire();
	BotStones B;
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Jeu de GOlmon");	//getDesktop crée une fenetre native de l'écran
	GoBoard C;
	PlayerStones P;
    sf::Vector2u a = window.getSize();	//on utilise getSize pour que notre tableau soit fait en fonction de la taille de notre fenetre
	B.BoardToVector(a,m);

	afficherMatrice(m);
    C.draw(a);
	window.clear(sf::Color(200 , 173 , 127));
	C.display(window);
	while (window.isOpen())
	{
		a = window.getSize();
		sf::Event event;
		while (window.pollEvent(event))
		{	
			if (event.type == sf::Event::Closed){
				window.close();
				
			}
            if (event.type == sf::Event::Resized){	//Resized permet d'avoir une fenetre plutot responsive
                window.clear(sf::Color(200, 173, 127));
				C.display(window);
            }
			if (event.type == sf::Event::MouseButtonPressed){
				a = window.getSize();
				//std::cout<<a.x<<std::endl<<a.y<<std::endl;
				sf::Vector2i pos = sf::Mouse::getPosition(window);
				window.clear(sf::Color(200,173,127));
				C.display(window);
				P.display(window,pos,a,m);
				afficherMatrice(m);
			}
		}
		B.display(window);
		window.display();
	}
return 0;
}
