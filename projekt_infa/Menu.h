#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
private:
    std::vector<sf::Text> menuText;
    int selectedIndex;
    sf::Font& font;

public:
    Menu(const std::vector<std::string>& items, sf::Font& font, float startY, float spacing, sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedIndex() const;
    void render(sf::RenderWindow& window) const;
};

#endif // MENU_H