#ifndef PACMAN_H
#define PACMAN_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Maze.h"

using namespace sf;

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
    Pacman(float startX, float startY, float radius, float speed);

    void move(const Maze& maze);
    void animateMouth();
    void updateShape();
    void renderPacman(RenderWindow& window) const;
    bool checkCollisionWithGhost(Vector2f ghostPosition, float ghostRadius) const;
    void update(const Maze& maze);
};

#endif // PACMAN_H
