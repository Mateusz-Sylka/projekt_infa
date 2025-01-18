#include "Ghost.h"

Ghost::Ghost(float startX, float startY, Color color, float speed, float radius)
    : position(startX, startY), speed(speed) {
    ghostHead.setRadius(radius);
    ghostHead.setFillColor(color);
    ghostHead.setOrigin(radius, radius);
    ghostHead.setPosition(position);

    ghostLegs.setSize(Vector2f(2 * radius, radius));
    ghostLegs.setFillColor(color);
    ghostLegs.setOrigin(ghostLegs.getSize().x / 2, 0);
    ghostLegs.setPosition(position);
}

void Ghost::changeDirection(Keyboard::Key key) {
    if (key == Keyboard::Right) {
        velocity = { speed, 0 };
        ghostLegs.setRotation(270);
        ghostHead.setRotation(270);
    }
    else if (key == Keyboard::Left) {
        velocity = { -speed, 0 };
        ghostLegs.setRotation(90);
        ghostHead.setRotation(90);
    }
    else if (key == Keyboard::Up) {
        velocity = { 0, -speed };
        ghostLegs.setRotation(180);
        ghostHead.setRotation(180);
    }
    else if (key == Keyboard::Down) {
        velocity = { 0, speed };
        ghostLegs.setRotation(0);
        ghostHead.setRotation(0);
    }
}

Vector2f Ghost::getPosition() const {
    return position;
}


float Ghost::getRadius() const {
    return ghostHead.getRadius();
}

void Ghost::move(Maze& maze, int& score) {
    Vector2f newPosition = position + velocity;

    if (maze.checkSpikeCollision(newPosition, ghostHead.getRadius())) {
        std::cout << "Game Over! Ghost hit a spike!" << std::endl;
        exit(0); // Exit the game
    }

    if (maze.isWalkable(newPosition, ghostHead.getRadius())) {
        position = newPosition;
    }

    if (maze.checkCoinCollision(position, ghostHead.getRadius())) {
        std::cout << "Ghost collected a coin!" << std::endl;
        score += 10; // Increment the score directly
        std::cout << "score is" << score << std::endl;
    }

    ghostHead.setPosition(position);
    ghostLegs.setPosition(position);
}

void Ghost::render(RenderWindow& window) const {
    window.draw(ghostLegs);
    window.draw(ghostHead);
}
void Ghost::setPosition(float x, float y) {
    position = { x, y };
    ghostHead.setPosition(position);
    ghostLegs.setPosition(position);
}