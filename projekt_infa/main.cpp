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
#include "TextLabel.h"

using namespace sf;

enum class GameState {
    Menu,
    Playing,
    Exiting,
    Help,
    Settings
};

void centerText(Text& text, RenderWindow& window) {
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    text.setPosition(window.getSize().x / 2.0f, text.getPosition().y);
}



int main() {            // Main function
    Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf"))
        return -1;

    GameState currentState = GameState::Menu;

    const float tileSize = 41;
    const float screenHeight = 11 * tileSize;
    const float screenWidth = 20 * tileSize;


    RenderWindow window(VideoMode(screenWidth, screenHeight), "Extra gra :)");

    std::vector<std::string> menuItems = { "Start Game", "Options", "Exit","Help"};       //menu items
    std::vector<Text> menuText;


    for (size_t i = 0; i < menuItems.size(); ++i) {
        Text text(menuItems[i], font, 40);
        text.setFillColor(sf::Color::White);
        text.setPosition(400, 200 + i * 60); // Adjust y-spacing
        centerText(text, window);
        menuText.push_back(text);
    }
    int selectedMenuIndex = 0;
    menuText[selectedMenuIndex].setFillColor(Color::Yellow);


    // Define the maze layout
    Maze maze1(mazeLayout1, tileSize);
    Maze maze2(mazeLayout2, tileSize);
    Maze maze3(mazeLayout3, tileSize);
    Maze maze4(mazeLayout4, tileSize);
    std::vector<Maze> mazes = { maze1, maze2, maze3,maze4 };

    Color ghostColor = Color::White;                                                                 //start of settings adjusting
    TextLabel ustawienia("Wybierz kolor duszka", font);
    Ghost yellowGhost(screenWidth / 5, screenHeight * 3 / 5, Color::Yellow, 0, 30);
    Ghost greenGhost(screenWidth * 2 / 5, screenHeight * 3 / 5, Color::Green, 0, 30);
    Ghost whiteGhost(screenWidth * 3 / 5, screenHeight * 3 / 5, Color::White, 0, 30);
    Ghost brownGhost(screenWidth * 4 / 5, screenHeight * 3 / 5, Color(139, 69, 19), 0, 30);
    std::vector<Ghost> ghosts = { yellowGhost, greenGhost, whiteGhost, brownGhost };
    int selectedSettingsIndex = 0;
    ghosts[selectedSettingsIndex].setRadius(40);                                                     //end of settings adjusting

    std::string help = "Up - goes up \nDown - goes down \nRight - goes right \n Left - goes left \nescape - Quit game";
    TextLabel pomoc(help,font);
    TextLabel pomoc2("Help: ", font);

    Pacman pacman(tileSize * 12.5f, tileSize * 1.5f, tileSize / 2.f - 0.05f, 0.06f);
    Ghost ghost(10.5 * tileSize, 9.5 * tileSize, ghostColor, 1.f, tileSize / 2.f - 1.f);


    int currentLevel = 0;
    int currentScore = 0;
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
                else if (event.key.code == Keyboard::F1 && currentState==GameState::Playing) {
                    currentState = GameState::Help;
                }
                else if (currentState == GameState::Menu) {                 //menu changing loop
                    if (event.key.code == Keyboard::Up) {
                        // Move selection up
                        menuText[selectedMenuIndex].setFillColor(sf::Color::White);
                        selectedMenuIndex = (selectedMenuIndex - 1 + menuItems.size()) % menuItems.size();
                        menuText[selectedMenuIndex].setFillColor(sf::Color::Yellow);
                    }
                    else if (event.key.code == Keyboard::Down) {
                        // Move selection down
                        menuText[selectedMenuIndex].setFillColor(sf::Color::White);
                        selectedMenuIndex = (selectedMenuIndex + 1) % menuItems.size();
                        menuText[selectedMenuIndex].setFillColor(sf::Color::Yellow);
                    }
                    else if (event.key.code == Keyboard::Enter) {
                        if (selectedMenuIndex == 0) {

                            std::cout << "Starting Game..." << std::endl;
                            currentState = GameState::Playing;
                        }
                        else if (selectedMenuIndex == 1) {
                            // Options
                            std::cout << "Opening Options..." << std::endl;
                            currentState = GameState::Settings;
                        }
                        else if (selectedMenuIndex == 2) {
                            currentState = GameState::Exiting;
                            std::cout << "Exiting?" << std::endl;

                        }
                        else if (selectedMenuIndex == 3)
                            currentState = GameState::Help;
                    }
                }
                else if (currentState == GameState::Playing) {
                    // Handle game-specific controls
                    ghost.changeDirection(event.key.code);  // Change direction on key press
                }
                else if (currentState == GameState::Settings) {         //settings changing loop
                    if (event.key.code == Keyboard::Left) {
                        // Move selection to the left
                        ghosts[selectedSettingsIndex].setRadius(30);
                        selectedSettingsIndex = (selectedSettingsIndex - 1 + ghosts.size()) % ghosts.size();
                        ghosts[selectedSettingsIndex].setRadius(40);
                    }
                    else if (event.key.code == Keyboard::Right) {
                        // Move selection to the left             
                        ghosts[selectedSettingsIndex].setRadius(30);
                        selectedSettingsIndex = (selectedSettingsIndex + 1) % ghosts.size();
                        ghosts[selectedSettingsIndex].setRadius(40);
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        switch (selectedSettingsIndex) {
                        case 0:
                            ghostColor = Color::Yellow;
                            break;
                        case 1:
                            ghostColor = Color::Green;
                            break;
                        case 2:
                            ghostColor = Color::White;
                            break;
                        case 3:
                            ghostColor = Color(139, 69, 19);
                            break;
                        }                       
                        currentState = GameState::Menu; // Return to menu
                    }
                }
               
            }
            if (event.type == Event::KeyReleased) {
               if (event.key.code == Keyboard::F1 && currentState == GameState::Help) {
                        currentState = GameState::Playing; 
                    }
                }
        }
            window.clear();

            if (currentState == GameState::Menu) {
                // Render the menu
                for (const auto& text : menuText)
                    window.draw(text);
            }
            else if (currentState == GameState::Exiting) {
                window.close();
            }
            else if (currentState == GameState::Playing) {
                // Render the game      
                mazes[currentLevel].render(window);
                pacman.renderPacman(window);
                ghost.render(window);
                ghost.setColor(ghostColor);
                // Update movements
                ghost.move(mazes[currentLevel], currentScore);
                pacman.update(mazes[currentLevel]);

                // Check for collisions
                if (pacman.checkCollisionWithGhost(ghost.getPosition(), ghost.getRadius())) {
                    std::cout << "Collision! Ghost is caught!" << std::endl;
                    ghost.setPosition(tileSize * 10.5f, tileSize * 9.5f);

                }

                // Check for warp gate collisions and move to next level
                if (mazes[currentLevel].checkWarpGateCollision(ghost.getPosition(), ghost.getRadius())) {
                    std::cout << "Warp gate used! Moving to next level..." << std::endl;
                    currentLevel = (currentLevel + 1) % totalLevels;  // Wrap around to the first level if at the end

                    // Reset positions for the new level
                    pacman.setPosition(Vector2f(tileSize * 12.5f, tileSize * 1.5f));  // Example starting position for Pacman
                    ghost.setPosition(tileSize * 10.5f, tileSize * 9.5f);    // Example starting position for Ghost
                }
            }
            else if (currentState == GameState::Help) {
                pomoc.render(window);
                pomoc2.render(window);
                pomoc.centerOrigin();
                pomoc.setPosition(screenWidth / 2.f, screenHeight / 2.f);
                pomoc.setSize(30);
    
            }
            else if (currentState == GameState::Settings) {
                ustawienia.render(window);
                ustawienia.centerOrigin();
                ustawienia.setPosition(screenWidth / 2, screenHeight / 4);
                for (auto& yellowGhost : ghosts) {
                    yellowGhost.render(window);
                }
            }

            window.display();
        
    }
}
