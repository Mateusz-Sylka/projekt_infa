#include "Settings.h"

Settings::Settings(std::vector<Ghost>& ghosts, Font& font, float screenWidth, float screenHeight)
    : ghosts(ghosts), selectedIndex(0), title("Choose Ghost Color", font) {
    title.centerOrigin();
    title.setPosition(screenWidth / 2, screenHeight / 4);
    ghosts[selectedIndex].setRadius(40); // Highlight the first ghost
}

void Settings::moveLeft() {
    ghosts[selectedIndex].setRadius(30);
    selectedIndex = (selectedIndex - 1 + ghosts.size()) % ghosts.size();
    ghosts[selectedIndex].setRadius(40);
}

void Settings::moveRight() {
    ghosts[selectedIndex].setRadius(30);
    selectedIndex = (selectedIndex + 1) % ghosts.size();
    ghosts[selectedIndex].setRadius(40);
}

Color Settings::getSelectedColor() const {
    return ghosts[selectedIndex].getColor();
}

void Settings::render(RenderWindow& window) const {
    title.render(window);
    for (const auto& ghost : ghosts) {
        ghost.render(window);
    }
}
