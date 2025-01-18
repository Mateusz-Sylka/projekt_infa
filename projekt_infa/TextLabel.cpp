#include "TextLabel.h"
#include <stdexcept>

TextLabel::TextLabel(const std::string& labelText, Font& font,    float x, float y, unsigned int size, Color color)
    {

    text.setFont(font);
    text.setString(labelText);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(x, y);
}

void TextLabel::setText(const std::string& newText) {
    text.setString(newText);
}

void TextLabel::setPosition(float x, float y) {
    text.setPosition(x, y);
}

void TextLabel::centerOrigin() {
    // Get the text's local bounds
    FloatRect bounds = text.getLocalBounds();

    // Set the origin to the center of the text
    text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

void TextLabel::setSize(int size) {
    text.setCharacterSize(size);
}


void TextLabel::render(RenderWindow& window) const {
    window.draw(text);
}
