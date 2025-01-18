#ifndef TEXTLABEL_H
#define TEXTLABEL_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

using namespace sf;

class TextLabel {
private:
    Text text;

public:
    // Constructor
    TextLabel(const std::string& labelText, Font& font, float x = 0.f, float y = 0.f, unsigned int size = 30,
        Color color = Color::White);

    // Setters
    void setText(const std::string& newText);
    void setPosition(float x, float y);
    void centerOrigin();
    void setSize(int size);

    // Rendering
    void render(RenderWindow& window) const;
};

#endif // TEXTLABEL_H
