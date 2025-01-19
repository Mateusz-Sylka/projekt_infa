#include "Coins.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
using namespace sf;

    Coin::Coin(float x, float y, float radius)
        : collected(false) {
        shape.setRadius(radius);
        shape.setFillColor(Color::Yellow);
        shape.setOrigin(radius, radius);
        shape.setPosition(x, y);
    }

    // Render the coin
    void Coin::render(RenderWindow& window) const {
        if (!collected) {
            window.draw(shape);
        }
    }

    // Check if the coin has been collected
    bool Coin::isCollected() const {
        return collected;
    }

    // Check if ghost collects the coin
    bool Coin::checkCollision(Vector2f colliderPosition, float colliderRadius) {
        if (!collected) {
            Vector2f coinPosition = shape.getPosition();
            float distance = std::sqrt(std::pow(colliderPosition.x - coinPosition.x, 2) +
                std::pow(colliderPosition.y - coinPosition.y, 2));
            if (distance < colliderRadius + shape.getRadius()) {
                collected = true;
                return true;
            }
        }
        return false;
    }
