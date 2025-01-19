#ifndef HELPSCREEN_H
#define HELPSCREEN_H

#include <SFML/Graphics.hpp>
#include "TextLabel.h"
using namespace sf;

class HelpScreen {
private:
    TextLabel helpTitle;
    TextLabel helpContent;

public:
    HelpScreen(const std::string& helpText, Font& font, float screenWidth, float screenHeight);
    void render(RenderWindow& window) const;
};

#endif // HELPSCREEN_H
