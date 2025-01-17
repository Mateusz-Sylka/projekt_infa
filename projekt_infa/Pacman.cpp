#include "Pacman.h"

Pacman::Pacman(float startX, float startY, float radius, float speed)
    : position(startX, startY), speed(speed), mouthOpen(true), pacRadius(radius), velocity(speed, 0) {
    pacBody.setPointCount(32);
    pacBody.setFillColor(Color::Yellow);
    pacBody.setOrigin(radius, radius);
    pacBody.setPosition(position);
    updateShape();
}

void Pacman::move(const Maze& maze) {
    Vector2f newPosition = position + velocity;

    if (!maze.isWalkable(newPosition, pacRadius)) {
        velocity.x = -velocity.x;
        pacBody.rotate(180);
    }
    else {
        position = newPosition;
    }
    pacBody.setPosition(position);
}

void Pacman::animateMouth() {
    if (animationClock.getElapsedTime().asMilliseconds() > 200) {
        mouthOpen = !mouthOpen;
        animationClock.restart();
        updateShape();
    }
}

void Pacman::updateShape() {
    int startAngle = mouthOpen ? 30 : 0;
    int endAngle = mouthOpen ? 330 : 360;

    int numPoints = 2 + (endAngle - startAngle);
    pacBody.setPointCount(numPoints);

    pacBody.setPoint(0, { pacRadius, pacRadius });

    int index = 1;
    for (int angle = startAngle; angle <= endAngle; ++angle) {
        float radian = angle * 3.14159f / 180.f;
        float x = pacRadius + pacRadius * std::cos(radian);
        float y = pacRadius + pacRadius * std::sin(radian);
        pacBody.setPoint(index++, { x, y });
    }
}

void Pacman::renderPacman(RenderWindow& window) const {
    window.draw(pacBody);
}

bool Pacman::checkCollisionWithGhost(Vector2f ghostPosition, float ghostRadius) const {
    float distance = std::sqrt(std::pow(position.x - ghostPosition.x, 2) +
        std::pow(position.y - ghostPosition.y, 2));
    return distance < pacRadius + ghostRadius;
}

void Pacman::update(const Maze& maze) {
    move(maze);
    animateMouth();
}

void Pacman::setPosition(sf::Vector2f newPosition) {
    position = newPosition;
    pacBody.setPosition(position);
}

void Pacman::setRadius(float newRadius) {
    pacRadius = newRadius;
    pacBody.setOrigin(pacRadius, pacRadius);
    updateShape(); // Recalculate the shape with the new radius
}

float Pacman::getRadius() const {
    return pacRadius;
}

Vector2f Pacman::getPosition() const {
    return position;
}