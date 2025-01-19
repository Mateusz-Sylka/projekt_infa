#include "HelpScreen.h"

HelpScreen::HelpScreen(const std::string& helpText, Font& font, float screenWidth, float screenHeight)
    : helpTitle("Help", font), helpContent(helpText, font) {
    helpTitle.centerOrigin();
    helpTitle.setPosition(screenWidth / 2, screenHeight / 5);

    helpContent.setSize(30);
    helpContent.centerOrigin();
    helpContent.setPosition(screenWidth / 2, screenHeight / 2);
}

void HelpScreen::render(RenderWindow& window) const {
    helpTitle.render(window);
    helpContent.render(window);
}
