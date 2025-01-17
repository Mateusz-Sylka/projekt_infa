#include "Spike.h"

    Spike::Spike(float x, float y, float size, int orientation)
        : tileSize(size) {
        // Define spike shape
        spike.setPointCount(3);
        spike.setPoint(0, { 0, size });
        spike.setPoint(1, { size / 2, 0 });
        spike.setPoint(2, { size, size });
        spike.setFillColor(Color::Red);
        spike.setOrigin(size / 2, size / 2);
        spike.setPosition(x, y);

        // Rotate spike based on orientation
        // 0: Up, 1: Right, 2: Down, 3: Left
        if (orientation == 0) {
            spike.setRotation(0); // Pointing up
        }
        else if (orientation == 1) {
            spike.setRotation(90); // Pointing right
        }
        else if (orientation == 2) {
            spike.setRotation(180); // Pointing down
        }
        else if (orientation == 3) {
            spike.setRotation(270); // Pointing left
        }
    }

    void  Spike::render(RenderWindow& window) const {
        window.draw(spike);
    }

    bool  Spike::checkCollision(Vector2f colliderPosition, float colliderRadius) const {
        FloatRect spikeBounds = spike.getGlobalBounds();
        Vector2f spikeCenter(spikeBounds.left + spikeBounds.width / 2,
            spikeBounds.top + spikeBounds.height / 2);
        float distance = std::sqrt(std::pow(colliderPosition.x - spikeCenter.x, 2) +
            std::pow(colliderPosition.y - spikeCenter.y, 2));
        return distance < colliderRadius + tileSize / 2;
    }
