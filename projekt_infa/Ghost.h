#ifndef GHOST_H
#define GHOST_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Maze.h"

using namespace sf;

class Ghost {
private:
    CircleShape ghostHead;
    RectangleShape ghostLegs;
    Vector2f position;
    float speed;
    Vector2f velocity;

public:
    Ghost(float startX, float startY, Color color, float speed, float radius);

    void changeDirection(Keyboard::Key key);
    Vector2f getPosition() const;
    float getRadius() const;
    void move(Maze& maze);
    void render(RenderWindow& window) const;
    void setPosition(float x, float y);

};

#endif // GHOST_H
