#ifndef MAZE_H
#define MAZE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "WarpGate.h"
#include "Coins.h"
#include "Spike.h"

using namespace sf;

class Maze {
private:
    WarpGate warpGate;
    std::vector<std::vector<int>> layout;
    float tileSize;
    Color wallColor;
    std::vector<Coins> coins;
    std::vector<Spike> spikes;

public:
    Maze(const std::vector<std::vector<int>>& layout, float tileSize, Color wallColor = Color::Magenta);

    void render(RenderWindow& window) const;
    void initializeWarpGate();
    bool checkWarpGateCollision(Vector2f colliderPosition, float colliderRadius);
    void initializeSpikes();
    bool checkSpikeCollision(Vector2f colliderPosition, float colliderRadius) const;
    bool checkCoinCollision(Vector2f colliderPosition, float colliderRadius);
    void initializeCoins();
    bool isWalkable(Vector2f position, float radius) const;
};

#endif // MAZE_H