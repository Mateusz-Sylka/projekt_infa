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
    std::vector<std::vector<int>> mazeLayout1 = {
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 0, 0, 0, 1, 1, 2, 0, 4, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
        {0, 0, 1, 0, 0, 1, 1, 0, 2, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 1, 2, 2, 1, 1, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 1, 2, 2, 1, 0, 2, 0, 1, 2, 1, 1, 1, 0, 0, 0, 0, 2, 1},
        {0, 0, 1, 2, 2, 1, 2, 1, 2, 1, 2, 2, 2, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 1, 0, 2, 2, 0, 2, 0, 1, 2, 0, 2, 1, 0, 0, 3, 3, 3, 1},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };
    std::vector<std::vector<int>> mazeLayout2 = {
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {0, 1, 0, 0, 0, 1, 1, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
      {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
      {0, 0, 1, 0, 0, 1, 1, 0, 2, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 1, 2, 2, 1, 1, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 1, 2, 2, 1, 0, 2, 0, 2, 2, 1, 1, 1, 0, 0, 0, 0, 2, 1},
      {0, 0, 1, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 1, 0, 2, 2, 0, 2, 2, 2, 2, 0, 2, 1, 0, 0, 3, 3, 3, 1},
      {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };
    std::vector<std::vector<int>> mazeLayout3 = {
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {0, 1, 0, 0, 0, 1, 1, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
      {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
      {0, 0, 1, 0, 0, 1, 1, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 1, 2, 2, 1, 1, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 1, 2, 2, 1, 0, 2, 0, 2, 2, 0, 1, 1, 0, 0, 0, 0, 2, 1},
      {0, 0, 1, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 1, 0, 0, 2, 0, 2, 2, 2, 2, 0, 2, 1, 0, 0, 3, 3, 3, 1},
      {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    Maze maze1(mazeLayout1, tileSize);
    Maze maze2(mazeLayout2, tileSize);
    Maze maze3(mazeLayout3, tileSize);
    Pacman pacman(tileSize * 12.5f, tileSize * 1.5f, tileSize / 2.f - 0.05f, 0.06f);
    Ghost ghost(10.5*tileSize, 9.5*tileSize, Color::White, 1.f, tileSize / 2.f - 1.f);
    Maze labirynt(mazeLayout1, tileSize);
    std::vector<Maze> mazes = { maze1, maze2, maze3};
    int currentLevel = 0;


    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    window.close();
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
                    pacman.update(maze1);
                    ghost.move(maze1);
                    ghost.changeDirection(event.key.code);

                    if (pacman.checkCollisionWithGhost(ghost.getPosition(), ghost.getRadius())) 
                    {
                        std::cout << "Collision! Pac-Man is caught!" << std::endl;
                        window.close();
                    }
                    else if (maze1.checkWarpGateCollision(ghost.getPosition(), ghost.getRadius()))
                    {
                        window.clear();
                        maze2.render(window);
                    }

                    
                }
            }
        }

        window.clear();

        if (currentState == GameState::Menu) {
            // Render the menu
            for (const auto& text : menuText)
                window.draw(text);
        }
        else if (currentState == GameState::Playing) {
            // Render the game
            maze1.render(window);
            pacman.renderPacman(window);
            ghost.render(window);
        }

        window.display();
    }


    return 0;
}           
/*maze1.render(window);
pacman.update(maze1);
ghost.move(maze1);
ghost.render(window);
pacman.renderPacman(window);
window.display();
if (maze1.checkWarpGateCollision(ghost.getPosition(), ghost.getRadius())) {
    window.clear();

    maze2.render(window);
    window.display();
}
if (pacman.checkCollisionWithGhost(ghost.getPosition(), ghost.getRadius())) {
    std::cout << "Collision! Pac-Man is caught!" << std::endl;
    window.close();
}*/