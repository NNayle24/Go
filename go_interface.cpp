/* Compiler avec -lsfml-graphics -lsfml-window -lsfml-system
   TODO :
          -Recommenter/rephraser parce que c'est très cuisiné par GPT pour responsive et no flashes
          -Bouton pass, bouton resign sous le plateau
          -Le flow du actual jeu, pour l'instant : EVENT=click(x,y) -> isValidPosition->modif matrice tableau -> UpdateBoard -> display -> tour IA -> display
*/
#include <SFML/Graphics.hpp>
#include <iostream>

//Including C files
extern "C"{
	#include "const.h"
    #include "board.h"
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
    void updateStones(sf::Vector2u windowSize, char** board) {
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


class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;

public:
    Button(float x, float y, float width, float height, const std::string& str, sf::Color bgColor = sf::Color::White, sf::Color textColor = sf::Color::Black) {
        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(bgColor);
        shape.setOutlineColor(sf::Color::Black); // Add an outline
        shape.setOutlineThickness(2);

        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Could not load font\n";
        }

        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(24); // Adapt ?
        text.setFillColor(textColor);
        
        // Center the text on the button
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0, textRect.top + textRect.height / 2.0);
        text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f, shape.getPosition().y + shape.getSize().y / 2.0f - 10); // Slightly adjust to center
        
    }

    void display(sf::RenderWindow& window) {
        window.draw(shape); // Draw the rectangle background
        window.draw(text);  // Ensure text is drawn after the shape
    }

    bool isClicked(sf::Vector2f mousePos) {
        return shape.getGlobalBounds().contains(mousePos);
    }
};
void refreshDisplay(sf::RenderWindow& window, GoBoard& goBoard, Button& passButton, Button& resignButton) {
    window.clear(sf::Color(128, 128, 128));            
    goBoard.display(window);
    passButton.display(window);
    resignButton.display(window);
    window.display();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Go Game", sf::Style::Resize | sf::Style::Close);
    Item itm = createItem();
    char **board = itm->board; //pointer to the Item board, should be fine modifying its value to modify Item's values

    int X =0 ,Y=0; //AI's chosen coordinates

    GoBoard goBoard;
    Button passButton(0, 0, 100, 50, "Pass", sf::Color(200, 200, 200));
    Button resignButton(0, 0, 100, 50, "Resign", sf::Color(200, 200, 200));
    
    while (window.isOpen()) 
    {

        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            } 
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
            {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                float a = 0.85f * window.getSize().y / BOARD_SIZE;
                float offset = window.getSize().x / 2 - 0.85f * window.getSize().y / 2;
                float clickTolerance = a * 0.25;  // Tolerance of 25% of the cell size


                if (passButton.isClicked(mousePos)) {
                    std::cout << "Pass button clicked" << std::endl;
                    if(calculate_scores(board)>0.5){
                        std::cout << "White wins" << std::endl;
                    }
                    else{
                        std::cout << "Black wins" << std::endl;
                        window.close();
                    }
                }
                else if (resignButton.isClicked(mousePos)) {
                    std::cout << "Resign button clicked" << std::endl;
                } 
                else 
                {
                    for (int i = 0; i < BOARD_SIZE; ++i) 
                    {
                        for (int j = 0; j < BOARD_SIZE; ++j) 
                        {
                            float stoneCenterX = offset + i * a;
                            float stoneCenterY = 0.1f * window.getSize().y + j * a;
                            if (std::abs(mousePos.x - stoneCenterX) < clickTolerance && 
                                std::abs(mousePos.y - stoneCenterY) < clickTolerance && 
                                board[i][j] == 0) 
                                {
                                if(IsValidPosition(itm, i, j))
                                {
                                    board[i][j] = -1;  // Player places a stone, modify the board
                                    UpdateBoard(itm, i, j);        
                                    refreshDisplay(window, goBoard, passButton, resignButton);
                                    IA_computing(board, &X, &Y); //AI returns its move as pointers
                                    board[X][Y] = 1; //AI places a stone, modify the board
                                    UpdateBoard(itm, X, Y);
                                    refreshDisplay(window, goBoard, passButton, resignButton);
                                }
                            }
                        }
                    }
                }
            }

            else if (event.type == sf::Event::Resized) {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                goBoard.draw(sf::Vector2u(event.size.width, event.size.height));
                goBoard.updateStones(sf::Vector2u(event.size.width, event.size.height), board);
                if (event.type == sf::Event::Resized || event.type == sf::Event::GainedFocus) {
                // Update button positions based on window size
                float buttonWidth = 100;
                float buttonHeight = 40;
                float boardBottom = 0.1f * window.getSize().y + BOARD_SIZE * (0.85f * window.getSize().y / BOARD_SIZE);
                float middle = window.getSize().x / 2.0f;

                passButton = Button(middle - buttonWidth - 10, boardBottom + 20 - buttonHeight, buttonWidth, buttonHeight, "Pass", sf::Color(200, 200, 200));
                resignButton = Button(middle + 10, boardBottom + 20 - buttonHeight, buttonWidth, buttonHeight, "Resign", sf::Color(200, 200, 200));
            }
            }
            refreshDisplay(window, goBoard, passButton, resignButton);
        }
    }

    // for (int i = 0; i < BOARD_SIZE; ++i) {
    //     delete[] board[i];
    // }
    // delete[] board;

    return 0;
}
