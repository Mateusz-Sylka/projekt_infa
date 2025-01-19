#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace sf;

class Exiting {
private:
    Font& font;
    Text promptText;
    std::vector<Text> options;
    int selectedIndex;
    float screenWidth;
    float screenHeight;

    void updateSelection();

public:
    Exiting(Font& font, float screenWidth, float screenHeight);

    void moveUp();
    void moveDown();
    bool isYesSelected() const;

    void render(RenderWindow& window);
};