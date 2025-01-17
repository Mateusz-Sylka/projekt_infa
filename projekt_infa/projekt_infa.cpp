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


int main() {            // Main function
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) 
        return -1; // Handle font loading error
    
    const float tileSize = 41;
    const float screenHeight = 11 * tileSize;
    const float screenWidth = 20 * tileSize;

    RenderWindow window(VideoMode(static_cast<unsigned int>(screenWidth), static_cast<unsigned int>(screenHeight)), "Pacman Game");

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

    Maze maze1(mazeLayout1, tileSize);
    Maze maze2(mazeLayout2, tileSize);
    Pacman pacman(tileSize * 12.5f, tileSize * 1.5f, tileSize / 2.f - 0.05f, 0.06f);
    Ghost ghost(10.5*tileSize, 9.5*tileSize, Color::White, 1.f, tileSize / 2.f - 1.f);
    Maze labirynt(mazeLayout1, tileSize);
   

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape)
                    window.close();
                ghost.changeDirection(event.key.code);
            }
          /*  if (event.type == sf::Event::KeyPressed) {
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
                        // Replace with your game start logic
                        std::cout << "Starting Game..." << std::endl;

                    }
                    else if (selectedIndex == 1) {
                        // Options
                        // Replace with your options logic
                        std::cout << "Opening Options..." << std::endl;
                    }
                    else if (selectedIndex == 2) {
                        // Exit
                        window.close();
                    }
                }
            }*/
        }
        window.clear();
      //  for (const auto& text : menuText) 
       //     window.draw(text);
        
        
        maze1.render(window);
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
            window.close(); // End the game or reset logic here
        }
        
       
    }

    return 0;
}           
