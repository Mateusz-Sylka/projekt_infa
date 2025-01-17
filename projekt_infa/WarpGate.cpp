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
        // Get the center of the gate
        Vector2f gateCenter = gate.getPosition();

        // Calculate the squared distance between the collider and the gate center
        float deltaX = colliderPosition.x - gateCenter.x;
        float deltaY = colliderPosition.y - gateCenter.y;
        float distanceSquared = deltaX * deltaX + deltaY * deltaY;

        // Calculate the sum of the radii
        float combinedRadii = colliderRadius + gate.getSize().x / 2;

        // Compare squared distances to avoid using the expensive square root function
        return distanceSquared < combinedRadii * combinedRadii;
    }

    bool WarpGate::isActivated() const {
        return activated;
    }

    void WarpGate::activate() {
        activated = true;
    }