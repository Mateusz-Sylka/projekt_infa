#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <math.h> 

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
        pacBody.setPointCount(32); 
        pacBody.setFillColor(Color::Yellow); 
        pacBody.setOrigin(radius, radius); // Środek jako punkt odniesienia
        pacBody.setPosition(position);
        updateShape(); 
    }

    // Funkcja do zmiany kierunku ruchu
    void changeDirection(Keyboard::Key key) {
        if (key == Keyboard::Up) {
            velocity = { 0, -speed };
            pacBody.setRotation(270);
        }
        else if (key == Keyboard::Down) {
            velocity = { 0, speed };
            pacBody.setRotation(90);
        }
        else if (key == Keyboard::Left) {
            velocity = { -speed, 0 };
            pacBody.setRotation(180);
        }
        else if (key == Keyboard::Right) {
            velocity = { speed, 0 };
            pacBody.setRotation(0);
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
        int startAngle = mouthOpen ? 30 : 0;  
        int endAngle = mouthOpen ? 330 : 360; 

        int numPoints = 2 + (endAngle - startAngle); 
        pacBody.setPointCount(numPoints); // Set the number of points

        
        pacBody.setPoint(0, { pacRadius, pacRadius });

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
    Pacman pacman(screenWidth / 2, screenHeight / 2, 25.f, 0.1f);

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