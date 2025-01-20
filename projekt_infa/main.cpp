#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <numeric>

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
{
    while (true) // Main game loop
    {
        GameData gameData("savefile.txt");

        Font font;
        if (!font.loadFromFile("assets/fonts/arial.ttf"))
            return -1;

        GameState currentState = GameState::Menu;

        const float tileSize = 41;
        const float screenHeight = 11 * tileSize;
        const float screenWidth = 20 * tileSize;

        RenderWindow window(VideoMode(screenWidth, screenHeight), "Extra gra :)");

        std::vector<std::string> menuItems = { "Start Game","Load Game", "Options", "Exit","Help" };
        Menu menu(menuItems, font, 100, 60, window);

        // Define the maze layout
        Maze maze1(mazeLayout1, tileSize);
        Maze maze2(mazeLayout2, tileSize);
        Maze maze3(mazeLayout3, tileSize);
        Maze maze4(mazeLayout4, tileSize);
        std::vector<Maze> mazes = { maze1, maze2, maze3, maze4 };

        //Define Settings
        Color ghostColor = Color::White;
        TextLabel ustawienia("Wybierz kolor duszka", font);
        Ghost yellowGhost(screenWidth / 5, screenHeight * 3 / 5, Color::Yellow, 0, 30);
        Ghost greenGhost(screenWidth * 2 / 5, screenHeight * 3 / 5, Color::Green, 0, 30);
        Ghost whiteGhost(screenWidth * 3 / 5, screenHeight * 3 / 5, Color::White, 0, 30);
        Ghost brownGhost(screenWidth * 4 / 5, screenHeight * 3 / 5, Color(139, 69, 19), 0, 30);
        std::vector<Ghost> ghosts = { yellowGhost, greenGhost, whiteGhost, brownGhost };
        Settings settings(ghosts, font, screenWidth, screenHeight);

        //define HelpScreen
        std::string help = "Up - goes up \nDown - goes down \nRight - goes right \n Left - goes left \nEscape - Quit game";
        HelpScreen helpScreen(help, font, screenWidth, screenHeight);

        Exiting exitingScreen(font, screenWidth, screenHeight);

        Pacman pacman(tileSize * 12.5f, tileSize * 1.5f, tileSize / 2.f - 0.05f, 0.06f);
        Ghost ghost(10.5 * tileSize, 9.5 * tileSize, ghostColor, 0.5, tileSize / 2.f - 1.f);

        TextLabel scoreText("current score", font);
        scoreText.setPosition(0, screenHeight - 1 * tileSize);
        int currentScore = 0;
        std::vector<int> scores(mazes.size(), 0);

        TextLabel levelText("current level", font);
        levelText.setPosition(0, screenHeight - 2 * tileSize);
        int currentLevel = 0;
        int totalLevels = mazes.size();

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
                    else if (event.key.code == Keyboard::Escape && currentState == GameState::Menu)
                        currentState = GameState::Exiting;
                    else if (event.key.code == Keyboard::Escape && currentState == GameState::Settings)
                        currentState = GameState::Menu;
                    else if (event.key.code == Keyboard::Escape && currentState == GameState::Help)
                        currentState = GameState::Menu;
                    else if (event.key.code == Keyboard::Escape && currentState == GameState::Exiting)
                        window.close();
                    else if (event.key.code == Keyboard::F1 && currentState == GameState::Playing)
                        currentState = GameState::Help;
                    else if (event.key.code == Keyboard::Escape || event.key.code == Keyboard::Enter && currentState == GameState::Winning) {
                        window.close();
                        break; // Break the game loop to restart                   
                    }
                        
                        
                    else if (currentState == GameState::Menu)
                    {
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
                                if (gameData.getCurrentLevel() != 0) {
                                    gameData.load(scores, currentLevel);
                                    currentScore = scores[currentLevel];  // Load the score for the current level
                                    std::cout << "Loaded Score: " << currentScore << ", Level: " << currentLevel << std::endl;
                                    currentState = GameState::Playing;
                                    pacman.setPosition(Vector2f(tileSize * 12.5f, tileSize * 1.5f));
                                    ghost.setPosition(tileSize * 10.5f, tileSize * 9.5f);
                                }
                                else {
                                    std::cout << "No saved game to load." << std::endl;
                                }
                                break;
                            case 2: currentState = GameState::Settings; break;
                            case 3: currentState = GameState::Exiting; break;
                            case 4: currentState = GameState::Help; break;
                            }
                        }
                    }
                    else if (currentState == GameState::Playing)
                    {
                        ghost.changeDirection(event.key.code);
                    }
                    else if (currentState == GameState::Settings)
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
                    else if (currentState == GameState::Exiting)
                    {
                        if (event.key.code == Keyboard::Up)
                            exitingScreen.moveUp();
                        else if (event.key.code == Keyboard::Down)
                            exitingScreen.moveDown();
                        else if (event.key.code == Keyboard::Enter)
                        {
                            if (exitingScreen.isYesSelected())
                                window.close();
                            else
                                currentState = GameState::Menu;
                        }
                    }
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
                mazes[currentLevel].render(window);
                pacman.renderPacman(window);
                ghost.render(window);
                ghost.setColor(ghostColor);
                ghost.move(mazes[currentLevel]);
                pacman.update(mazes[currentLevel]);
                if (pacman.checkCollisionWithGhost(ghost.getPosition(), ghost.getRadius()))
                {
                    currentScore -= 50;
                    ghost.setPosition(tileSize * 10.5f, tileSize * 9.5f);
                }
                else if (mazes[currentLevel].checkCoinCollision(ghost.getPosition(), ghost.getRadius()))
                {
                    currentScore += 10;
                }
                else if (mazes[currentLevel].checkSpikeCollision(ghost.getPosition(), ghost.getRadius()))
                {
                    currentScore -= 50;
                    ghost.setPosition(tileSize * 10.5f, tileSize * 9.5f);
                }
                if (mazes[currentLevel].checkWarpGateCollision(ghost.getPosition(), ghost.getRadius())) {
                    scores[currentLevel] = currentScore; // Save the current score for this level
                    currentLevel = (currentLevel + 1);
                    gameData.save(currentScore, currentLevel);

                    
                    if (currentLevel >= totalLevels) {
                        currentState = GameState::Winning;
                    }
                    else {
                        pacman.setPosition(Vector2f(tileSize * 12.5f, tileSize * 1.5f));
                        ghost.setPosition(tileSize * 10.5f, tileSize * 9.5f);
                        currentScore = scores[currentLevel]; // Load the score for the next level
                    }
                }
                scoreText.render(window);
                levelText.render(window);
                levelText.setText("Level: " + std::to_string(currentLevel + 1));
                scoreText.setText("Score: " + std::to_string(currentScore));
            }
            else if (currentState == GameState::Winning)
            {
                // Calculate the total score from all levels
                int totalScore = std::accumulate(scores.begin(), scores.end(), 0);

                // Update the score text to show the total score
                scoreText.setText("Total Score: " + std::to_string(totalScore));
                scoreText.centerOrigin();
                scoreText.setPosition(screenWidth / 2, 50);

                // Render the winning message and total score
                Won.render(window);
                scoreText.render(window);
                next.render(window);
            }
            else if (currentState == GameState::Help)
                helpScreen.render(window);
            else if (currentState == GameState::Settings)
                settings.render(window);
            else if (currentState == GameState::Exiting)
                exitingScreen.render(window);

            window.display();
        }

        // If we broke out of the inner loop due to winning or user exiting, restart or close
        if (currentState != GameState::Winning)
            break; // Exit the outer loop and close the application
    }

    return 0;
}
