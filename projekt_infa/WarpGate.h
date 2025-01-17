#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <string>
using namespace sf;

class WarpGate
{
  
    private:
        RectangleShape gate;
        bool activated;

    public:
        WarpGate(float x, float y, float size, Color color); //constructor
     
        void render(RenderWindow& window) const;        //render WarpGate

        bool checkCollision(Vector2f colliderPosition, float colliderRadius);   //collision with player
           
        bool isActivated() const;       
        //going to next level
        void activate();                
       
};


