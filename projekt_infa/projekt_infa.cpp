#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <math.h>

using namespace sf;

// Maze class
class Maze {
private:
    std::vector<std::vector<int>> layout; 
    float tileSize;
    Color wallColor;

public:
    // Constructor
    Maze(const std::vector<std::vector<int>>& layout, float tileSize, Color wallColor = Color::Blue)
        : layout(layout), tileSize(tileSize), wallColor(wallColor) {
    }

    // Render maze
    void renderMaze(RenderWindow& window) const {
        for (size_t y = 0; y < layout.size(); ++y) {
            for (size_t x = 0; x < layout[y].size(); ++x) {
                if (layout[y][x] == 1) { //wall
                    RectangleShape wall(Vector2f(tileSize, tileSize));
                    wall.setFillColor(wallColor);
                    wall.setPosition(x * tileSize, y * tileSize);
                    window.draw(wall);
                }
            }
        }
    }

    bool isWalkable(Vector2f position, float radius) const {
        // Lista punktów do sprawdzenia (cztery punkty: góra, dół, lewo, prawo)
        std::vector<Vector2f> points = {
            {position.x - (radius), position.y},     // Lewy brzeg
            {position.x + (radius), position.y},     // Prawy brzeg
            {position.x, position.y - (radius)},     // Górny brzeg
            {position.x, position.y + (radius)}      // Dolny brzeg
        };

        // Sprawdzamy każdy punkt
        for (const auto& point : points) {
            int gridX = point.x / tileSize;
            int gridY = point.y / tileSize;

            // Jeśli punkt jest poza granicami lub w kolizji ze ścianą
            if (gridX < 0 || gridX >= layout[0].size() || gridY < 0 || gridY >= layout.size() || layout[gridY][gridX] != 0) {
                return false;
            }
        }

        return true; // Wszystkie punkty są przejezdne
    }
};

// Pacman class
class Pacman {
private:
    ConvexShape pacBody;
    Vector2f position;
    Vector2f velocity;
    float speed;
    bool mouthOpen;
    Clock animationClock;
    float pacRadius;

public:
    // Constructor
    Pacman(float startX, float startY, float radius, float speed)
        : position(startX, startY), speed(speed), mouthOpen(true), pacRadius(radius) {
        pacBody.setPointCount(32);
        pacBody.setFillColor(Color::Yellow);
        pacBody.setOrigin(radius, radius);
        pacBody.setPosition(position);
        updateShape();
    }

    // Change direction
    void changeDirection(Keyboard::Key key) {
        if (key == Keyboard::Up) {
            velocity = { 0, -speed };
            pacBody.setRotation(270);
        }
        else if (key == Keyboard::Down) {
            velocity = { 0, speed };
            pacBody.setRotation(90);
        }
        else if (key == Keyboard::Left) {
            velocity = { -speed, 0 };
            pacBody.setRotation(180);
        }
        else if (key == Keyboard::Right) {
            velocity = { speed, 0 };
            pacBody.setRotation(0);
        }
    }

    // Move Pac-Man
    void move(const Maze& maze) {
        Vector2f newPosition = position + velocity;

        // Sprawdź, czy wszystkie punkty wokół Pac-Mana (jego narożniki) mogą się poruszyć
        if (maze.isWalkable(newPosition, pacRadius)) {
            position = newPosition; // Aktualizacja pozycji, jeśli przejście jest możliwe
        }

        pacBody.setPosition(position); // Ustaw nową pozycję Pac-Mana
    }

    // Animate Pac-Man's mouth
    void animateMouth() {
        if (animationClock.getElapsedTime().asMilliseconds() > 200) {
            mouthOpen = !mouthOpen;
            animationClock.restart();
            updateShape();
        }
    }

    // Update Pac-Man's shape dziala juz nie zmieniam tego
    void updateShape() {
        int startAngle = mouthOpen ? 30 : 0;
        int endAngle = mouthOpen ? 330 : 360;

        int numPoints = 2 + (endAngle - startAngle);
        pacBody.setPointCount(numPoints);

        pacBody.setPoint(0, { pacRadius, pacRadius });

        int index = 1;
        for (int angle = startAngle; angle <= endAngle; ++angle) {
            float radian = angle * 3.14159f / 180.f;
            float x = pacRadius + pacRadius * cos(radian);
            float y = pacRadius + pacRadius * sin(radian);
            pacBody.setPoint(index++, { x, y });
        }
    }

    // Render Pac-Man
    void renderPacman(RenderWindow& window) {
        window.draw(pacBody);
    }

    // Update Pac-Man
    void update(const Maze& maze) {
        move(maze);
        animateMouth();
    }
};

class duch {
private:
    CircleShape duchHEAD;
    RectangleShape duchLEGS;
    Vector2f positionD;
    float speedD;
    Vector2f velocityD;
    
public:
    duch(float startX, float startY, Color color, float speed, float Dradius) :
        positionD(startX, startY), speedD(speed) {

        duchHEAD.setRadius(Dradius);            //Duchhead
        duchHEAD.setFillColor(color);
        duchHEAD.setOrigin(duchHEAD.getRadius(), duchHEAD.getRadius());
        duchHEAD.setPosition(positionD);

        duchLEGS.setSize(Vector2f(2*duchHEAD.getRadius(), duchHEAD.getRadius()));       //duchlegs
        duchLEGS.setFillColor(color);
        duchLEGS.setOrigin(duchLEGS.getSize().x / 2, 0);
        duchLEGS.setPosition(positionD);
    }
    // Change direction
    void D_changeDirection(Keyboard::Key key) {
        if (key == Keyboard::Right) {
            velocityD = { 0, -speedD };
            duchLEGS.setRotation(270);
            duchHEAD.setRotation(270);
        }
        else if (key == Keyboard::Left) {
            velocityD = { 0, speedD };
            duchLEGS.setRotation(90);
            duchHEAD.setRotation(90);
        }
        else if (key == Keyboard::Up) {
            velocityD = { -speedD, 0 };
            duchLEGS.setRotation(180);
            duchHEAD.setRotation(180);
        }
        else if (key == Keyboard::Down) {
            velocityD = { speedD, 0 };
            duchLEGS.setRotation(0);
            duchHEAD.setRotation(0);
        }
    }






    // Render Duch
    void renderDuch(RenderWindow& window) {
        window.draw(duchLEGS);
        window.draw(duchHEAD);
    }
};

// Main function
int main() {
    const float screenWidth = 800;
    const float screenHeight = 600;
    const float tileSize = 41;

    RenderWindow window(VideoMode(screenWidth, screenHeight), "Pacman Game");

    // Define the maze layout
    std::vector<std::vector<int>> mazeLayout = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1},
        {1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    Maze maze(mazeLayout, tileSize);
    Pacman pacman(tileSize*1.5, tileSize*1.5, ((tileSize/2)-0.05), 0.06);
    duch P1(screenWidth / 2, screenHeight / 2,Color::White,10,tileSize/2);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape)
                    window.close();
                pacman.changeDirection(event.key.code);
                P1.D_changeDirection(event.key.code);
            }
        }

        window.clear();
        maze.renderMaze(window);
        pacman.update(maze);
        P1.renderDuch(window);
        pacman.renderPacman(window);
        window.display();
    }

    return 0;
}
