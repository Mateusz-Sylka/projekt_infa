#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace sf;

class Menu {
private:
    std::vector<Text> menuText;
    int selectedIndex;
   Font& font;

public:
    Menu(const std::vector<std::string>& items, Font& font, float startY, float spacing, RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedIndex() const;
    void render(RenderWindow& window) const;
};

#endif // MENU_H