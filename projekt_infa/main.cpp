#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "Coins.h"
#include "WarpGate.h"
#include "Spike.h"
#include "Maze.h"
#include "Pacman.h"
#include "Ghost.h"
#include "MazeLayouts.h"

using namespace sf;

void centerText(Text& text, RenderWindow& window) {
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    text.setPosition(window.getSize().x / 2.0f, text.getPosition().y);
}

enum class GameState {
    Menu,
    Playing,
    Exiting,
    Help
};


int main() {            // Main function
    Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) 
        return -1;

    GameState currentState = GameState::Menu;
    
    const float tileSize = 41;
    const float screenHeight = 11 * tileSize;
    const float screenWidth = 20 * tileSize;
  

    RenderWindow window(VideoMode(screenWidth, screenHeight), "Extra gra :)");

    std::vector<std::string> menuItems = { "Start Game", "Options", "Exit" };       //menu items
    std::vector<sf::Text> menuText;


    for (size_t i = 0; i < menuItems.size(); ++i) {
        sf::Text text(menuItems[i], font, 40);
        text.setFillColor(sf::Color::White);
        text.setPosition(400, 200 + i * 60); // Adjust y-spacing
        centerText(text, window);
        menuText.push_back(text);
    }
    int selectedIndex = 0;
    menuText[selectedIndex].setFillColor(sf::Color::Yellow);


    // Define the maze layout
    Maze maze1(mazeLayout1, tileSize);
    Maze maze2(mazeLayout2, tileSize);
    Maze maze3(mazeLayout3, tileSize);
    Maze maze4(mazeLayout4, tileSize);
    std::vector<Maze> mazes = { maze1, maze2, maze3,maze4 };


    Pacman pacman(tileSize * 12.5f, tileSize * 1.5f, tileSize / 2.f - 0.05f, 0.06f);
    Ghost ghost(10.5*tileSize, 9.5*tileSize, Color::White, 1.f, tileSize / 2.f - 1.f);
   
    int currentLevel = 0;
    int current_score = 0;
    int totalLevels = mazes.size();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    currentState = GameState::Exiting;                  
                }
                else if (currentState == GameState::Menu) {
                    // Handle menu navigation
                    if (event.key.code == sf::Keyboard::Up) {
                        // Move selection up
                        menuText[selectedIndex].setFillColor(sf::Color::White);
                        selectedIndex = (selectedIndex - 1 + menuItems.size()) % menuItems.size();
                        menuText[selectedIndex].setFillColor(sf::Color::Yellow);
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        // Move selection down
                        menuText[selectedIndex].setFillColor(sf::Color::White);
                        selectedIndex = (selectedIndex + 1) % menuItems.size();
                        menuText[selectedIndex].setFillColor(sf::Color::Yellow);
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        if (selectedIndex == 0) {
                            // Start Game
                            std::cout << "Starting Game..." << std::endl;
                            currentState = GameState::Playing;
                        }
                        else if (selectedIndex == 1) {
                            // Options
                            std::cout << "Opening Options..." << std::endl;
                        }
                        else if (selectedIndex == 2) {
                            // Exit
                            window.close();
                        }
                    }
                }
                else if (currentState == GameState::Playing) {
                    // Handle game-specific controls
                    ghost.changeDirection(event.key.code);  // Change direction on key press
                }
            }
        }
        window.clear();

        if (currentState == GameState::Menu) {
            // Render the menu
            for (const auto& text : menuText)
                window.draw(text);
        }
        else if(currentState == GameState::Exiting) {
            window.close();
        }
        else if (currentState == GameState::Playing) {
            // Render the game      
            mazes[currentLevel].render(window);
            pacman.renderPacman(window);
            ghost.render(window);

            // Update movements
            ghost.move(mazes[currentLevel]);
            pacman.update(mazes[currentLevel]);

            // Check for collisions
            if (pacman.checkCollisionWithGhost(ghost.getPosition(), ghost.getRadius())) {
                std::cout << "Collision! Pac-Man is caught!" << std::endl;
               // currentState = GameState::GameOver;  // You can transition to a game over state here
            }

            // Check for warp gate collisions and move to next level
            if (mazes[currentLevel].checkWarpGateCollision(pacman.getPosition(), pacman.getRadius())) {
                std::cout << "Warp gate used! Moving to next level..." << std::endl;
                currentLevel = (currentLevel + 1) % totalLevels;  // Wrap around to the first level if at the end

                // Reset positions for the new level
                pacman.setPosition(Vector2f(tileSize * 12.5f, tileSize * 1.5f));  // Example starting position for Pacman
                ghost.setPosition(tileSize * 10.5f, tileSize * 9.5f);    // Example starting position for Ghost
            }
        }
       // else if (currentState == GameState::GameOver) {
            // Render game over screen (not implemented here)
       // }

        window.display();
    }
}           
