/* Compiler avec -lsfml-graphics -lsfml-window -lsfml-system
   TODO :
          -Recommenter/rephraser parce que c'est très cuisiné par GPT pour responsive et no flashes
          -Bouton pass, bouton resign sous le plateau
          -Le flow du actual jeu, pour l'instant : EVENT=click(x,y) -> isValidPosition->modif matrice tableau -> UpdateBoard -> display -> tour IA -> display
*/
#include <SFML/Graphics.hpp>
#include <iostream>

//for random, remove both once tests are over
#include <cstdlib>
#include <ctime>

//Including C files
extern "C"{
	#include "const.h"
	#include "struct.h"
}


class GoBoard 
{
private:
    sf::VertexArray hlines, vlines;
    std::vector<sf::CircleShape> stones; // Stores both player and bot stones

public:
    GoBoard() : hlines(sf::Lines), vlines(sf::Lines) {}

    void draw(sf::Vector2u windowSize)
    {
        hlines.clear();
        vlines.clear();
        float lineSpacing = 0.85f * windowSize.y / BOARD_SIZE;
        float offset = windowSize.x / 2 - 0.85f * windowSize.y / 2;

        for (int i = 0; i < BOARD_SIZE; i++) {
            float pos = i * lineSpacing;
            hlines.append(sf::Vertex(sf::Vector2f(offset, 0.1f * windowSize.y + pos), sf::Color::Black));
            hlines.append(sf::Vertex(sf::Vector2f(offset + (BOARD_SIZE - 1) * lineSpacing, 0.1f * windowSize.y + pos), sf::Color::Black));

            vlines.append(sf::Vertex(sf::Vector2f(offset + pos, 0.1f * windowSize.y), sf::Color::Black));
            vlines.append(sf::Vertex(sf::Vector2f(offset + pos, 0.1f * windowSize.y + (BOARD_SIZE - 1) * lineSpacing), sf::Color::Black));
        }
    }

    // Update the stones vector based on the board state
    void updateStones(sf::Vector2u windowSize, int** board) {
        stones.clear();
        float radius = 0.85f * windowSize.y / (2 * BOARD_SIZE);
        float a = 0.85f * windowSize.y / BOARD_SIZE;
        float offset = windowSize.x / 2 - 0.85f * windowSize.y / 2;

        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] != 0) {
                    sf::CircleShape stone(radius);
                    stone.setOrigin(radius, radius);
                    stone.setPosition(offset + i * a, 0.1f * windowSize.y + j * a);
                    stone.setFillColor(board[i][j] == 1 ? sf::Color::White : sf::Color::Black);
                    stones.push_back(stone);
                }
            }
        }
    }

    void display(sf::RenderWindow& window) {
        window.draw(hlines);
        window.draw(vlines);
        for (auto& stone : stones) {
            window.draw(stone);
        }
    }
};

int** createRandomBoard() {
    int** board = new int*[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        board[i] = new int[BOARD_SIZE];
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = std::rand() % 3 - 1;  // Generates -1, 0, 1 randomly
        }
    }
    return board;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    sf::RenderWindow window(sf::VideoMode(800, 600), "Go Game", sf::Style::Resize | sf::Style::Close);

    int** board = createRandomBoard();

    GoBoard goBoard;

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            } 
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                float a = 0.85f * window.getSize().y / BOARD_SIZE;
                float offset = window.getSize().x / 2 - 0.85f * window.getSize().y / 2;
                float clickTolerance = a * 0.25;  // Tolerance of 25% of the cell size

                for (int i = 0; i < BOARD_SIZE; ++i) {
                    for (int j = 0; j < BOARD_SIZE; ++j) {
                        float stoneCenterX = offset + i * a;
                        float stoneCenterY = 0.1f * window.getSize().y + j * a;
                        if (std::abs(mousePos.x - stoneCenterX) < clickTolerance && 
                            std::abs(mousePos.y - stoneCenterY) < clickTolerance && 
                            board[i][j] == 0) {
                            board[i][j] = 1;  // Player places a stone
                            goBoard.updateStones(window.getSize(), board);
                        }
                    }
                }
            }

            else if (event.type == sf::Event::Resized) {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                goBoard.draw(sf::Vector2u(event.size.width, event.size.height));
                goBoard.updateStones(sf::Vector2u(event.size.width, event.size.height), board);
            }
            window.clear(sf::Color(128, 128, 128));
            goBoard.display(window);
            window.display();
        }
    }

    for (int i = 0; i < BOARD_SIZE; ++i) {
        delete[] board[i];
    }
    delete[] board;

    return 0;
}
