#ifndef SETTINGS_H
#define SETTINGS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Ghost.h"
#include "TextLabel.h"

using namespace sf;

class Settings {
private:
    std::vector<Ghost>& ghosts;
    int selectedIndex;
    TextLabel title;

public:
    Settings(std::vector<Ghost>& ghosts, Font& font, float screenWidth, float screenHeight);
    void moveLeft();
    void moveRight();
    Color getSelectedColor() const;
    void render(RenderWindow& window) const;
};

#endif // SETTINGS_H
