#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace sf;
class WarpGate {
private:
    RectangleShape gate;
    bool activated;

public:
    WarpGate(float x, float y, float size, Color color)
        : activated(false) {
        gate.setSize({ size, size });
        gate.setFillColor(color);
        gate.setOrigin(size / 2, size / 2);
        gate.setPosition(x, y);
    }

    void render(RenderWindow& window) const {
        window.draw(gate);
    }

    bool checkCollision(Vector2f colliderPosition, float colliderRadius) {
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

    bool isActivated() const {
        return activated;
    }

    void activate() {
        activated = true;
    }
};

class Spike {
private:
    ConvexShape spike;
    float tileSize;

public:
    Spike(float x, float y, float size, int orientation)
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

    void render(RenderWindow& window) const {
        window.draw(spike);
    }

    bool checkCollision(Vector2f colliderPosition, float colliderRadius) const {
        FloatRect spikeBounds = spike.getGlobalBounds();
        Vector2f spikeCenter(spikeBounds.left + spikeBounds.width / 2,
            spikeBounds.top + spikeBounds.height / 2);
        float distance = std::sqrt(std::pow(colliderPosition.x - spikeCenter.x, 2) +
            std::pow(colliderPosition.y - spikeCenter.y, 2));
        return distance < colliderRadius + tileSize / 2;
    }
};

class Coin {
private:
    CircleShape shape;
    bool collected;

public:
    Coin(float x, float y, float radius)
        : collected(false) {
        shape.setRadius(radius);
        shape.setFillColor(Color::Yellow);
        shape.setOrigin(radius, radius);
        shape.setPosition(x, y);
    }

    // Render the coin
    void render(RenderWindow& window) const {
        if (!collected) {
            window.draw(shape);
        }
    }

    // Check if the coin has been collected
    bool isCollected() const {
        return collected;
    }

    // Check if ghost collects the coin
    bool checkCollision(Vector2f colliderPosition, float colliderRadius) {
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
};

// Maze class
class Maze {
private:
    WarpGate warpGate; 
    std::vector<std::vector<int>> layout;
    float tileSize;
    Color wallColor;
    std::vector<Coin> coins;
    std::vector<Spike> spikes; // Store spikes in the maze

public:
    Maze(const std::vector<std::vector<int>>& layout, float tileSize, Color wallColor = Color::Magenta)
        : layout(layout), tileSize(tileSize), wallColor(wallColor),
        warpGate(0, 0, tileSize, Color::Cyan) { // Initialize warp gate with default values
        initializeCoins();
        initializeSpikes();
        initializeWarpGate(); // Initialize warp gate
    }

    // Render maze
    // Render maze including spikes
    void render(RenderWindow& window) const {
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
    void initializeWarpGate() {
        for (size_t y = 0; y < layout.size(); ++y) {
            for (size_t x = 0; x < layout[y].size(); ++x) {
                if (layout[y][x] == 4) { // 4 denotes warp gate in layout
                    warpGate = WarpGate((x + 0.5f) * tileSize, (y + 0.5f) * tileSize, tileSize, Color::Cyan);
                    break;
                }
            }
        }
    }
    bool checkWarpGateCollision(Vector2f colliderPosition, float colliderRadius) {
        return warpGate.checkCollision(colliderPosition, colliderRadius);
    }

    void initializeSpikes() {
        for (size_t y = 0; y < layout.size(); ++y) {
            for (size_t x = 0; x < layout[y].size(); ++x) {
                if (layout[y][x] == 3) { // Use 3 to denote spike in the layout
                    int orientation = 0;

                    // Check neighboring cells to determine orientation
                    if (y > 0 && layout[y - 1][x] == 1) orientation = 2; // Point down if wall above
                    else if (y < layout.size() - 1 && layout[y + 1][x] == 1) orientation = 0; // Point up if wall below
                    else if (x > 0 && layout[y][x - 1] == 1) orientation = 1; // Point right if wall to the left
                    else if (x < layout[y].size() - 1 && layout[y][x + 1] == 1) orientation = 3; // Point left if wall to the right
                    else if (x < layout[y].size() - 1 && layout[y][x + 1] == 1 && y < layout.size() - 1 && layout[y + 1][x] == 1) orientation = 0; //right down - point up
                    else if (y > 0 && layout[y - 1][x] == 1 && x < layout[y].size() - 1 && layout[y][x + 1] == 1) orientation = 2;//up-right - point down
                    else if (x < layout[y].size() - 1 && layout[y][x + 1] == 1 && y < layout.size() - 1 && layout[y + 1][x] == 1) orientation = 0; // left-down - point up

                    spikes.emplace_back((x + 0.5f) * tileSize, (y + 0.5f) * tileSize, tileSize, orientation);
                }
            }
        }
    }

    // Check for spike collisions
    bool checkSpikeCollision(Vector2f colliderPosition, float colliderRadius) const {
        for (const auto& spike : spikes) {
            if (spike.checkCollision(colliderPosition, colliderRadius)) {
                return true; // Spike collision detected
            }
        }
        return false;
    }


    // Check for coin collisions
    bool checkCoinCollision(Vector2f colliderPosition, float colliderRadius) {
        for (auto& coin : coins) {
            if (coin.checkCollision(colliderPosition, colliderRadius)) {
                return true; // Coin is collected
            }
        }
        return false;
    }

    void initializeCoins() {
        for (size_t y = 0; y < layout.size(); ++y) {
            for (size_t x = 0; x < layout[y].size(); ++x) {
                if (layout[y][x] == 2) { // Place coins where layout is 2
                    coins.emplace_back((x + 0.5f) * tileSize, (y + 0.5f) * tileSize, tileSize / 4.f);
                }
            }
        }
    }

    bool isWalkable(Vector2f position, float radius) const {
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
};

// Pacman class
class Pacman {
private:
    ConvexShape pacBody;
    Vector2f position;
    Vector2f velocity;
    float speed;
    bool mouthOpen;
    Clock animationClock;
    float pacRadius;

public:
    // Constructor
    Pacman(float startX, float startY, float radius, float speed)
        : position(startX, startY), speed(speed), mouthOpen(true), pacRadius(radius), velocity(speed, 0) {
        pacBody.setPointCount(32);
        pacBody.setFillColor(Color::Yellow);
        pacBody.setOrigin(radius, radius);
        pacBody.setPosition(position);
        updateShape();
    }

    // Move Pac-Man
    void move(const Maze& maze) {
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

    // Animate Pac-Man's mouth
    void animateMouth() {
        if (animationClock.getElapsedTime().asMilliseconds() > 200) {
            mouthOpen = !mouthOpen;
            animationClock.restart();
            updateShape();
        }
    }

    // Update Pac-Man's shape
    void updateShape() {
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

    // Render Pac-Man
    void renderPacman(RenderWindow& window) const {
        window.draw(pacBody);
    }

    // Update Pac-Man
    void update(const Maze& maze) {
        move(maze);
        animateMouth();
    }
};

class Ghost {
private:
    CircleShape ghostHead;
    RectangleShape ghostLegs;
    Vector2f position;
    float speed;
    Vector2f velocity;

public:
    Ghost(float startX, float startY, Color color, float speed, float radius)
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

    // Change direction
    void changeDirection(Keyboard::Key key) {
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
    Vector2f getPosition() const {
        return position;
    }

    float getRadius() const {
        return ghostHead.getRadius();
    }

    void move(Maze& maze) {
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
        }

        ghostHead.setPosition(position);
        ghostLegs.setPosition(position);
    }

    // Render Ghost
    void render(RenderWindow& window) const {
        window.draw(ghostLegs);
        window.draw(ghostHead);
    }
};

// Main function
int main() {
    const float tileSize = 41;
    const float screenHeight = 11 * tileSize;
    const float screenWidth = 20 * tileSize;

    RenderWindow window(VideoMode(static_cast<unsigned int>(screenWidth), static_cast<unsigned int>(screenHeight)), "Pacman Game");

    // Define the maze layout
    std::vector<std::vector<int>> mazeLayout = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 2, 0, 4, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1},
        {1, 2, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3, 3, 3, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    Maze maze(mazeLayout, tileSize);
    Pacman pacman(tileSize * 1.5f, tileSize * 1.5f, tileSize / 2.f - 0.05f, 0.06f);
    Ghost ghost(10.5*tileSize, 8.5*tileSize, Color::White, 1.f, tileSize / 2.f - 1.f);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape)
                    window.close();
                ghost.changeDirection(event.key.code);
            }
            if (maze.checkWarpGateCollision(ghost.getPosition(), ghost.getRadius())) {
                std::cout << "Warp gate activated! Transitioning to the next level..." << std::endl;
                // Load next level or change layout here
            }
        }

        window.clear();
        maze.render(window);
        pacman.update(maze);
        ghost.move(maze);
        ghost.render(window);
        pacman.renderPacman(window);
        window.display();
    }

    return 0;
}
