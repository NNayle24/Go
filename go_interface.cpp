/* Compiler avec -lsfml-graphics -lsfml-window -lsfml-system */
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
public:
    sf::Texture textureNormal;
    sf::Sprite sprite;
    sf::FloatRect bounds;

    // Constructor to load textures and initialize the button
    Button(const std::string& normalPath, float x, float y, float width, float height) {
        textureNormal.loadFromFile(normalPath);

        sprite.setTexture(textureNormal);
        sprite.setPosition(x, y);
        sprite.setScale(width / sprite.getLocalBounds().width, height / sprite.getLocalBounds().height);
        updateBounds(); // Initialize bounds
    }

    void updateBounds() {
        bounds = sprite.getGlobalBounds(); // Update the bounding box for click detection
    }

    bool isClicked(const sf::Vector2f& mousePos) {
        return bounds.contains(mousePos);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};


void endGame(sf::RenderWindow& window, bool whiteWins) {
    // Load the end game image
    sf::Texture endGameTexture;
    if (!endGameTexture.loadFromFile("endgame.png")) {
        std::cerr << "Failed to load end game image!" << std::endl;
        return;
    }
    sf::Sprite endGameSprite;
    endGameSprite.setTexture(endGameTexture);

    // Find height, screen/10
    float scale = window.getSize().y / (10.0f * endGameTexture.getSize().y);
    endGameSprite.setScale(scale, scale);

    // Horizontal position
    float spriteCenterX = (window.getSize().x - endGameTexture.getSize().x * scale) / 2;

    // Vertical position
    float spriteCenterY = (window.getSize().y / 2) - (endGameTexture.getSize().y * scale) - (window.getSize().y / 5);

    // Position the sprite
    endGameSprite.setPosition(spriteCenterX, spriteCenterY);


    // Compute winning stone
    sf::CircleShape winningStone(50);
    winningStone.setFillColor(whiteWins ? sf::Color::White : sf::Color::Black);

    // Center the circle on the window
    winningStone.setPosition(
        window.getSize().x / 2 - winningStone.getRadius(),
        window.getSize().y / 2 - winningStone.getRadius()
    );

    // Clear the window with a gray background
    window.clear(sf::Color(128, 128, 128));

    // Draw the sprite and the winning stone
    window.draw(winningStone);
    window.draw(endGameSprite);
    window.display();

    // Wait for the user to close the window
    sf::Event event;
    while (true) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
        }
    }
}




void refreshDisplay(sf::RenderWindow& window, GoBoard& goBoard, Button& passButton, Button& resignButton, char** board) {
    window.clear(sf::Color(128, 128, 128));     

    sf::Vector2u windowSize = window.getSize();
    goBoard.draw(windowSize);
    goBoard.updateStones(windowSize, board);
    goBoard.display(window);

    passButton.draw(window);
    resignButton.draw(window);
    window.display();
}



int main() {
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Le classiGO", sf::Style::Resize | sf::Style::Close);
    Item itm = createItem();
    char **board = itm->board; //pointer to the Item board, should be fine modifying its value to modify Item's values

    int X =0 ,Y=0; //AI's chosen coordinates

    GoBoard goBoard;
    // Button passButton("img.jpg", 0, 0, 100, 50);
    // Button resignButton("img.jpg", 0, 0, 100, 50);

    Button passButton("pass.png", 100, 550, 100, 50);
    Button resignButton("resign.png", 600, 550, 100, 50);
    sf::Vector2u windowSize;
    
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
                        endGame(window, true);
                        break;
                    }
                    else{
                        std::cout << "White refused end" << std::endl;
                    }
                }
                else if (resignButton.isClicked(mousePos)) {
                    std::cout << "Resign button clicked" << std::endl;
                    endGame(window, true);
                    break;
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
                                    std::cout<<"Computing..."<<std::endl;
                                    board[i][j] = -1;  // Player places a stone, modify the board
                                    UpdateBoard(itm, i, j);
                                    refreshDisplay(window, goBoard, passButton, resignButton, board);
                                    IA_computing(board, &X, &Y); //AI returns its move as pointers
                                    board[X][Y] = 1; //AI places a stone, modify the board
                                    UpdateBoard(itm, X, Y);
                                    printBoard(itm);
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

                passButton.sprite.setPosition(middle - 1.5f * buttonWidth, boardBottom + 10 - buttonHeight);
                resignButton.sprite.setPosition(middle + 0.5f * buttonWidth, boardBottom + 10 - buttonHeight);
                
                passButton.updateBounds();
                resignButton.updateBounds();
            }
            }
            refreshDisplay(window, goBoard, passButton, resignButton, board);
        }
        
    }

    // for (int i = 0; i < BOARD_SIZE; ++i) {
    //     delete[] board[i];
    // }
    // delete[] board;

    return 0;
}
