#include "WarpGate.h"

WarpGate::WarpGate(float x, float y, float size, Color color)
    : activated(false) {
    gate.setSize({ size, size });
    gate.setFillColor(color);
    gate.setOrigin(size / 2, size / 2);
    gate.setPosition(x, y);
}

void WarpGate::render(RenderWindow& window) const {
    window.draw(gate);
}

bool WarpGate::checkCollision(Vector2f colliderPosition, float colliderRadius) {
    // Get the center and size of the square gate
    Vector2f gateCenter = gate.getPosition();
    float halfSize = gate.getSize().x / 2; // Assuming gate is square

    // Calculate the bounds of the square, extended by the circle's radius
    float left = gateCenter.x - halfSize - colliderRadius;
    float right = gateCenter.x + halfSize + colliderRadius;
    float top = gateCenter.y - halfSize - colliderRadius;
    float bottom = gateCenter.y + halfSize + colliderRadius;

    // Check if the circle's center is within these bounds
    if (colliderPosition.x > left && colliderPosition.x < right &&
        colliderPosition.y > top && colliderPosition.y < bottom) {
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
