#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

#include "Coins.h"
#include "WarpGate.h"
#include "Spike.h"
#include "Maze.h"
#include "Pacman.h"
#include "Ghost.h"
#include "MazeLayouts.h"
#include "TextLabel.h"
#include "Menu.h"
#include "Settings.h"
#include "HelpScreen.h"
#include "Exiting.h"
#include "GameData.h"

using namespace sf;

enum class GameState {
    Menu,
    Playing,
    Exiting,
    Help,
    Settings,
    Winning
};


int main() 
{            // Main function

    GameData gameData("savefile.txt");

    Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf"))
        return -1;

    GameState currentState = GameState::Menu;



    const float tileSize = 41;
    const float screenHeight = 11 * tileSize;
    const float screenWidth = 20 * tileSize;

    RenderWindow window(VideoMode(screenWidth, screenHeight), "Extra gra :)");

    std::vector<std::string> menuItems = { "Start Game","Load Game", "Options", "Exit","Help" };       //menu items
    Menu menu(menuItems, font, 100, 60, window);

    // Define the maze layout
    Maze maze1(mazeLayout1, tileSize);
    Maze maze2(mazeLayout2, tileSize);
    Maze maze3(mazeLayout3, tileSize);
    Maze maze4(mazeLayout4, tileSize);
    std::vector<Maze> mazes = { maze1, maze2, maze3,maze4 };


    Color ghostColor = Color::White;                                                                     //settings adjusting
    TextLabel ustawienia("Wybierz kolor duszka", font);
    Ghost yellowGhost(screenWidth / 5, screenHeight * 3 / 5, Color::Yellow, 0, 30);
    Ghost greenGhost(screenWidth * 2 / 5, screenHeight * 3 / 5, Color::Green, 0, 30);
    Ghost whiteGhost(screenWidth * 3 / 5, screenHeight * 3 / 5, Color::White, 0, 30);
    Ghost brownGhost(screenWidth * 4 / 5, screenHeight * 3 / 5, Color(139, 69, 19), 0, 30);
    std::vector<Ghost> ghosts = { yellowGhost, greenGhost, whiteGhost, brownGhost };
    Settings settings(ghosts, font, screenWidth, screenHeight);


    std::string help = "Up - goes up \nDown - goes down \nRight - goes right \n Left - goes left \nEscape - Quit game"; //Help window
    HelpScreen helpScreen(help, font, screenWidth, screenHeight);

    Exiting exitingScreen(font, screenWidth, screenHeight);                     //exiting window

    Pacman pacman(tileSize * 12.5f, tileSize * 1.5f, tileSize / 2.f - 0.05f, 0.06f);
    Ghost ghost(10.5 * tileSize, 9.5 * tileSize, ghostColor, 1.f, tileSize / 2.f - 1.f);

    TextLabel scoreText("current score",font);                         //score counter
    scoreText.setPosition(0, screenHeight-1*tileSize); 
    int currentScore = 0;

    TextLabel levelText("current level", font);                     //level counter
    levelText.setPosition(0, screenHeight - 2*tileSize); 
    int currentLevel = 0; 
    int totalLevels = mazes.size();

    std::vector<bool> coinStates;

    TextLabel Won("You won!!!", font);
    Won.setPosition(screenWidth / 2, screenHeight / 2);
    Won.setSize(60);
    Won.centerOrigin();

    TextLabel next("Press enter to continue", font);
    next.centerOrigin();
    next.setPosition(screenWidth / 2, 300);


    while (window.isOpen()) 
    {
        Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) 
            {
                if (event.key.code == Keyboard::Escape && currentState == GameState::Playing)
                    currentState = GameState::Menu;                                                 
                else if (event.key.code == Keyboard::Escape && currentState == GameState::Menu)  //Escape for quiting
                    currentState = GameState::Exiting;
                else if (event.key.code == Keyboard::Escape && currentState == GameState::Settings)  
                    currentState = GameState::Menu;
                else if (event.key.code == Keyboard::Escape && currentState == GameState::Winning) 
                    currentState = GameState::Menu;
                else if (event.key.code == Keyboard::Escape && currentState == GameState::Help)  
                    currentState = GameState::Menu;

                else if (event.key.code == Keyboard::F1 && currentState == GameState::Playing)         //F1 for help
                    currentState = GameState::Help;
                
                else if (currentState == GameState::Menu)
                {                                                        //menu changing loop
                    if (event.key.code == Keyboard::Up) 
                        menu.moveUp();                
                    else if (event.key.code == Keyboard::Down) 
                        menu.moveDown();                
                    else if (event.key.code == Keyboard::Enter) 
                    {
                        switch (menu.getSelectedIndex()) 
                        {
                        case 0: currentState = GameState::Playing; break;
                        case 1: 
                            if (gameData.load(currentScore, currentLevel,coinStates))
                            {
                            std::cout << "Loaded Score: " << currentScore << ", Level: " << currentLevel << std::endl;
                            currentState = GameState::Playing;
                            pacman.setPosition(Vector2f(tileSize * 12.5f, tileSize * 1.5f)); // Reset Pacman position
                            ghost.setPosition(tileSize * 10.5f, tileSize * 9.5f);            // Reset Ghost position
                            }
                            else                          
                            std::cout << "No saved game to load." << std::endl;
                            
                            break;
                        case 2: currentState = GameState::Settings; break;
                        case 3: currentState = GameState::Exiting; break;
                        case 4: currentState = GameState::Help; break;
                        }
                    }

                }
                else if (currentState == GameState::Playing) {
                    // Handle game-specific controls
                    ghost.changeDirection(event.key.code);  // Ghost movement by pressing arrows
                }
                else if (currentState == GameState::Settings)       //settings changing loop
                {
                    if (event.key.code == Keyboard::Left)
                        settings.moveLeft();
                    else if (event.key.code == Keyboard::Right)
                        settings.moveRight();
                    else if (event.key.code == Keyboard::Enter)
                    {
                        ghostColor = settings.getSelectedColor();
                        currentState = GameState::Menu;      
                    }
                }
                 
                else if (currentState == GameState::Exiting)        //exiting changing loop
                {
                     if (event.key.code == Keyboard::Up) 
                            exitingScreen.moveUp();                      
                     else if (event.key.code == Keyboard::Down) 
                            exitingScreen.moveDown();
                        
                     else if (event.key.code == Keyboard::Enter)
                     {
                          if (exitingScreen.isYesSelected())
                             window.close(); // Close the game
                         else
                             currentState = GameState::Menu; // Return to menu
                     }
                }
                    else if (event.key.code == Keyboard::Escape && currentState == GameState::Menu) 
                        currentState = GameState::Exiting; // Trigger the Exiting state             
            }
            
                if (event.type == Event::KeyReleased) 
                {
                    if (event.key.code == Keyboard::F1 && currentState == GameState::Help) 
                    currentState = GameState::Playing;                
                }
        }
        window.clear();

        if (currentState == GameState::Menu) 
            menu.render(window);
              
        else if (currentState == GameState::Playing) 
        {
            // Render the game          
            mazes[currentLevel].render(window);
            pacman.renderPacman(window);
            ghost.render(window);
            ghost.setColor(ghostColor);
            // Update movements
            ghost.move(mazes[currentLevel]);
            pacman.update(mazes[currentLevel]);
            // Check for collisions
            if (pacman.checkCollisionWithGhost(ghost.getPosition(), ghost.getRadius())) 
            {
                currentScore -= 50;
                ghost.setPosition(tileSize * 10.5f, tileSize * 9.5f);
            }
            else if (mazes[currentLevel].checkCoinCollision(ghost.getPosition(), ghost.getRadius())) 
            {
                currentScore += 10;

            }
            else if(mazes[currentLevel].checkSpikeCollision(ghost.getPosition(), ghost.getRadius()))
            {
                currentScore -= 50;
                ghost.setPosition(tileSize * 10.5f, tileSize * 9.5f);
            }

            // Check for warp gate collisions and move to next level
            if (mazes[currentLevel].checkWarpGateCollision(ghost.getPosition(), ghost.getRadius())) 
            {
                currentLevel = (currentLevel + 1);  // Wrap around to the first level if at the end
                if (currentLevel == 4)
                    currentState = GameState::Winning;
                // Reset positions for the new level
                pacman.setPosition(Vector2f(tileSize * 12.5f, tileSize * 1.5f));  // Example starting position for Pacman
                ghost.setPosition(tileSize * 10.5f, tileSize * 9.5f);    // Example starting position for Ghost
            }
            scoreText.render(window);
            levelText.render(window);
            levelText.setText("Level: " + std::to_string(currentLevel+1));
            scoreText.setText("Score: " + std::to_string(currentScore));
        }
        else if (currentState == GameState::Winning) {
            Won.render(window);
            scoreText.centerOrigin();
            scoreText.setPosition(screenWidth / 2, 50);\
            scoreText.render(window);
            next.render(window);
        }
        else if (currentState == GameState::Help) 
            helpScreen.render(window);
        
        else if (currentState == GameState::Settings)
        {
            settings.render(window);
        }
        else if (currentState == GameState::Exiting)
        {
            exitingScreen.render(window);
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
            {
                if (exitingScreen.isYesSelected())
                {
                    gameData.save(currentScore, currentLevel,coinStates);
                    std::cout << "Game data saved on exit." << std::endl;
                    window.close();
                }
                else
                    currentState = GameState::Menu; // Return to menu
            }
        }

        window.display();

    }
}
