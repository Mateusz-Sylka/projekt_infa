#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <math.h> // do funkcji cos() i sin()

using namespace sf;

class Pacman {
private:
    ConvexShape pacBody;       // Kształt Pac-Mana
    Vector2f position;         // Pozycja
    Vector2f velocity;         // Prędkość
    float speed;               // Prędkość ruchu
    bool mouthOpen;            // Czy usta są otwarte
    Clock animationClock;      // Zegar do animacji
    float pacRadius;           // Promień Pac-Mana

public:
    // Konstruktor
    Pacman(float startX, float startY, float radius, float speed)
        : position(startX, startY), speed(speed), mouthOpen(true), pacRadius(radius) {
        pacBody.setPointCount(32); // Punktów wystarczająco dużo, aby Pac-Man wyglądał gładko
        pacBody.setFillColor(Color::Yellow); // Kolor żółty
        pacBody.setOrigin(radius, radius); // Środek jako punkt odniesienia
        pacBody.setPosition(position);
        updateShape(); // Ustaw początkowy kształt (usta otwarte)
    }

    // Funkcja do zmiany kierunku ruchu
    void changeDirection(Keyboard::Key key) {
        if (key == Keyboard::Up) {
            velocity = { 0, -speed };
        }
        else if (key == Keyboard::Down) {
            velocity = { 0, speed };
        }
        else if (key == Keyboard::Left) {
            velocity = { -speed, 0 };
        }
        else if (key == Keyboard::Right) {
            velocity = { speed, 0 };
        }
    }

    // Funkcja do ruchu
    void move(float screenWidth, float screenHeight) {
        position += velocity;

        // Wyjście za ekran (Pac-Man wraca na przeciwną stronę)
        if (position.x < 0) position.x = screenWidth;
        if (position.x > screenWidth) position.x = 0;
        if (position.y < 0) position.y = screenHeight;
        if (position.y > screenHeight) position.y = 0;

        pacBody.setPosition(position);
    }

    // Funkcja do animacji ust
    void animateMouth() {
        if (animationClock.getElapsedTime().asMilliseconds() > 200) {
            mouthOpen = !mouthOpen; // Przełącz stan ust (otwarte/zamknięte)
            animationClock.restart();
            updateShape(); // Zaktualizuj kształt
        }
    }

    // Funkcja do aktualizacji kształtu Pac-Mana
    void updateShape() {
        int startAngle = mouthOpen ? 15 : 0;  // Start angle (15° if mouth open, 0° if closed)
        int endAngle = mouthOpen ? 345 : 360; // End angle (345° if mouth open, 360° if closed)

        int numPoints = 2 + (endAngle - startAngle); // Total points (1 center + points on arc)
        pacBody.setPointCount(numPoints); // Set the number of points

        // Center point (for the wedge shape)
        pacBody.setPoint(0, { pacRadius, pacRadius });

        // Add points around the arc
        int index = 1; // Start from the first point after the center
        for (int angle = startAngle; angle <= endAngle; ++angle) {
            float radian = angle * 3.14159f / 180.f; // Convert to radians
            float x = pacRadius + pacRadius * cos(radian); // X-coordinate
            float y = pacRadius + pacRadius * sin(radian); // Y-coordinate
            pacBody.setPoint(index++, { x, y }); // Add the point and increment the index
        }
    }
    // Funkcja do rysowania Pac-Mana
    void render(RenderWindow& window) {
        window.draw(pacBody);
    }

    // Funkcja aktualizująca Pac-Mana (ruch + animacja)
    void update(float screenWidth, float screenHeight) {
        move(screenWidth, screenHeight);
        animateMouth();
    }
};

int main() {
    const float screenWidth = 800;
    const float screenHeight = 600;

    // Tworzymy okno
    RenderWindow window(VideoMode(screenWidth, screenHeight), "Pacman Game");

    // Tworzymy Pac-Mana
    Pacman pacman(screenWidth / 2, screenHeight / 2, 25.f, 0.2f);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape)
                    window.close();

                // Zmiana kierunku Pac-Mana
                pacman.changeDirection(event.key.code);
            }
        }

        window.clear();
        pacman.update(screenWidth, screenHeight); // Ruch i animacja Pac-Mana
        pacman.render(window);                   // Rysowanie Pac-Mana
        window.display();
    }

    return 0;
}