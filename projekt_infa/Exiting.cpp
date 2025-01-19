#include "Exiting.h"

Exiting::Exiting(Font& font, float screenWidth, float screenHeight)
    : font(font), selectedIndex(0), screenWidth(screenWidth), screenHeight(screenHeight) {
    // Set up the prompt text
    promptText.setFont(font);
    promptText.setString("Are you sure you want to quit?");
    promptText.setCharacterSize(30);
    promptText.setFillColor(Color::White);
    promptText.setPosition(screenWidth / 2.f, screenHeight / 3.f);
    promptText.setOrigin(promptText.getLocalBounds().width / 2.f, promptText.getLocalBounds().height / 2.f);

    // Set up the options
    std::vector<std::string> optionStrings = { "Yes", "No" };
    for (const auto& option : optionStrings) {
        Text optionText;
        optionText.setFont(font);
        optionText.setString(option);
        optionText.setCharacterSize(40);
        optionText.setFillColor(Color::White);
        options.push_back(optionText);
    }

    // Position options
    float spacing = 50.f;
    for (size_t i = 0; i < options.size(); ++i) {
        options[i].setPosition(screenWidth / 2.f, screenHeight / 2.f + i * spacing);
        options[i].setOrigin(options[i].getLocalBounds().width / 2.f, options[i].getLocalBounds().height / 2.f);
    }

    // Highlight the first option
    updateSelection();
}

void Exiting::moveUp() {
    if (selectedIndex > 0) {
        --selectedIndex;
        updateSelection();
    }
}

void Exiting::moveDown() {
    if (selectedIndex < options.size() - 1) {
        ++selectedIndex;
        updateSelection();
    }
}

void Exiting::updateSelection() {
    for (size_t i = 0; i < options.size(); ++i) {
        if (i == selectedIndex)
            options[i].setFillColor(Color::Yellow);
        else
            options[i].setFillColor(Color::White);
    }
}

bool Exiting::isYesSelected() const {
    return selectedIndex == 0;
}

void Exiting::render(RenderWindow& window) {
    window.draw(promptText);
    for (const auto& option : options)
        window.draw(option);
}
