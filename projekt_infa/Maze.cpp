#include "Maze.h"

Maze::Maze(const std::vector<std::vector<int>>& layout, float tileSize, Color wallColor)
    : layout(layout), tileSize(tileSize), wallColor(wallColor),
    warpGate(0, 0, tileSize, Color::Cyan) {
    initializeCoins();
    initializeSpikes();
    initializeWarpGate();
}

void Maze::render(RenderWindow& window) const {
    for (size_t y = 0; y < layout.size(); ++y) {
        for (size_t x = 0; x < layout[y].size(); ++x) {
            if (layout[y][x] == 1) { // Wall
                RectangleShape wall(Vector2f(tileSize, tileSize));
                wall.setFillColor(wallColor);
                wall.setPosition(static_cast<float>(x) * tileSize, static_cast<float>(y) * tileSize);
                window.draw(wall);
            }
        }
    }

    for (const auto& coin : coins)
        coin.render(window);

    for (const auto& spike : spikes)
        spike.render(window);

    warpGate.render(window); // Render warp gate
}

void Maze::initializeWarpGate() {
    for (size_t y = 0; y < layout.size(); ++y) {
        for (size_t x = 0; x < layout[y].size(); ++x) {
            if (layout[y][x] == 4) { // 4 denotes warp gate in layout
                warpGate = WarpGate((x + 0.5f) * tileSize, (y + 0.5f) * tileSize, tileSize, Color::Cyan);
                break;
            }
        }
    }
}

bool Maze::checkWarpGateCollision(Vector2f colliderPosition, float colliderRadius) {
    return warpGate.checkCollision(colliderPosition, colliderRadius);
}

void Maze::initializeSpikes() {
    for (size_t y = 0; y < layout.size(); ++y) {
        for (size_t x = 0; x < layout[y].size(); ++x) {
            if (layout[y][x] == 3) { // Use 3 to denote spike in the layout
                int orientation = 0;

                if (y > 0 && layout[y - 1][x] == 1) orientation = 2; // Point down if wall above
                else if (y < layout.size() - 1 && layout[y + 1][x] == 1) orientation = 0; // Point up if wall below
                else if (x > 0 && layout[y][x - 1] == 1) orientation = 1; // Point right if wall to the left
                else if (x < layout[y].size() - 1 && layout[y][x + 1] == 1) orientation = 3; // Point left if wall to the right
                else if (x < layout[y].size() - 1 && layout[y][x + 1] == 1 && y < layout.size() - 1 && layout[y + 1][x] == 1) orientation = 0; // right down - point up
                else if (y > 0 && layout[y - 1][x] == 1 && x < layout[y].size() - 1 && layout[y][x + 1] == 1) orientation = 2; // up-right - point down
                else if (x < layout[y].size() - 1 && layout[y][x + 1] == 1 && y < layout.size() - 1 && layout[y + 1][x] == 1) orientation = 0; // left-down - point up

                spikes.emplace_back((x + 0.5f) * tileSize, (y + 0.5f) * tileSize, tileSize, orientation);
            }
        }
    }
}

bool Maze::checkSpikeCollision(Vector2f colliderPosition, float colliderRadius) const {
    for (const auto& spike : spikes) {
        if (spike.checkCollision(colliderPosition, colliderRadius)) {
            return true; // Spike collision detected
        }
    }
    return false;
}

bool Maze::checkCoinCollision(Vector2f colliderPosition, float colliderRadius) {
    for (auto& coin : coins) {
        if (coin.checkCollision(colliderPosition, colliderRadius)) {
            return true; // Coin is collected
        }
    }
    return false;
}

void Maze::initializeCoins() {
    for (size_t y = 0; y < layout.size(); ++y) {
        for (size_t x = 0; x < layout[y].size(); ++x) {
            if (layout[y][x] == 2) { // Place coins where layout is 2
                coins.emplace_back((x + 0.5f) * tileSize, (y + 0.5f) * tileSize, tileSize / 4.f);
            }
        }
    }
}

bool Maze::isWalkable(Vector2f position, float radius) const {
    std::vector<Vector2f> points = {
        {position.x - radius, position.y},     // Left
        {position.x + radius, position.y},     // Right
        {position.x, position.y - radius},     // Top
        {position.x, position.y + radius}      // Bottom
    };

    for (const auto& point : points) {
        int gridX = static_cast<int>(point.x / tileSize);
        int gridY = static_cast<int>(point.y / tileSize);

        if (gridX < 0 || gridX >= static_cast<int>(layout[0].size()) ||
            gridY < 0 || gridY >= static_cast<int>(layout.size()) || layout[gridY][gridX] == 1) {
            return false; // Return false only if it's a wall (1)
        }
    }
    return true; // All points are walkable
}