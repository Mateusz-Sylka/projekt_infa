#include "WarpGate.h"

WarpGate::WarpGate(float x, float y, float size, sf::Color color)
    : activated(false) {
    gate.setSize({ size, size });
    gate.setFillColor(color);
    gate.setOrigin(size / 2, size / 2);
    gate.setPosition(x, y);
}

void WarpGate::render(sf::RenderWindow& window) const {
    window.draw(gate);
}

bool WarpGate::checkCollision(sf::Vector2f colliderPosition, float colliderRadius) {
    // Get the center of the gate
    sf::Vector2f gateCenter = gate.getPosition();

    // Calculate the squared distance between the collider and the gate center
    float deltaX = colliderPosition.x - gateCenter.x;
    float deltaY = colliderPosition.y - gateCenter.y;
    float distanceSquared = deltaX * deltaX + deltaY * deltaY;

    // Calculate the sum of the radii
    float combinedRadii = colliderRadius + gate.getSize().x / 2;

    // Compare squared distances to avoid using the expensive square root function
    if (distanceSquared < combinedRadii * combinedRadii) {
        activated = true; // Activate the gate on collision
        return true;      // Indicate a collision occurred
    }

    return false;
}

bool WarpGate::isActivated() const {
    return activated;
}

void WarpGate::activate() {
    activated = true;
}
