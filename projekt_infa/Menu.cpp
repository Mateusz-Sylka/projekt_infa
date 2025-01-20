#include "Menu.h"

Menu::Menu(const std::vector<std::string>& items, Font& font, float startY, float spacing, RenderWindow& window)
    : font(font), selectedIndex(0) {
    for (size_t i = 0; i < items.size(); ++i) {
       Text text(items[i], font, 40);
        text.setFillColor(Color::White);
        text.setPosition(300, startY + i * spacing);
       FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(window.getSize().x / 2.0f, text.getPosition().y);
        menuText.push_back(text);
    }
    menuText[selectedIndex].setFillColor(Color::Yellow);
}

void Menu::moveUp() {
    menuText[selectedIndex].setFillColor(Color::White);
    selectedIndex = (selectedIndex - 1 + menuText.size()) % menuText.size();
    menuText[selectedIndex].setFillColor(Color::Yellow);
}

void Menu::moveDown() {
    menuText[selectedIndex].setFillColor(Color::White);
    selectedIndex = (selectedIndex + 1) % menuText.size();
    menuText[selectedIndex].setFillColor(Color::Yellow);
}

int Menu::getSelectedIndex() const {
    return selectedIndex;
}

void Menu::render(RenderWindow& window) const {
    for (const auto& text : menuText)
        window.draw(text);
}
