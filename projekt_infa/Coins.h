#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
using namespace sf;

class Coins
{
private:
    CircleShape shape;
    bool collected;

public:
    Coins(float x, float y, float radius); //constuctor
       
           
    // Render the coin
    void render(RenderWindow& window) const;
       

    // Check if the coin has been collected
    bool isCollected() const;
 
    // Check if ghost collects the coin
    bool checkCollision(Vector2f colliderPosition, float colliderRadius);
};

