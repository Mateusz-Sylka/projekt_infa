#include "Ghost.h"

Ghost::Ghost(float startX, float startY, Color color, float speed, float radius)
    : position(startX, startY), speed(speed), isMoving(false) { // Initialize isMoving to false
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
    if (!isMoving) { // Only allow changing direction if not moving
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
        isMoving = true; // Set the flag to true as the ghost starts moving
    }
}

Vector2f Ghost::getPosition() const {
    return position;
}

float Ghost::getRadius() const {
    return ghostHead.getRadius();
}

void Ghost::move(Maze& maze) {
    Vector2f newPosition = position + velocity;

    if (maze.isWalkable(newPosition, ghostHead.getRadius())) {
        position = newPosition;
    }
    else {
        velocity = { 0, 0 }; // Stop the ghost if it can't move further
        isMoving = false;    // Allow changing direction again
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
    isMoving = false; // Reset the movement flag when setting a new position
}

void Ghost::setRadius(float GhostRadius) {
    ghostHead.setRadius(GhostRadius);
    ghostLegs.setSize(Vector2f(2 * GhostRadius, GhostRadius));
    ghostLegs.setOrigin(ghostLegs.getSize().x / 2, 0);
    ghostHead.setOrigin(GhostRadius, GhostRadius);
}

void Ghost::setColor(Color ghostColor) {
    ghostLegs.setFillColor(ghostColor);
    ghostHead.setFillColor(ghostColor);
}

Color Ghost::getColor() const {
    return ghostHead.getFillColor();
}
