#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
using namespace sf;

class Spike {
private:
    ConvexShape spike;
    float tileSize;

public:
    Spike(float x, float y, float size, int orientation); // constuctor with rotation system

    void render(RenderWindow& window) const;        //render spikes

    bool checkCollision(Vector2f colliderPosition, float colliderRadius) const;     //collison with player
 
};
