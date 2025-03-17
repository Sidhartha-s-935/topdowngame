#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
class TextBox {
private:
    sf::RectangleShape background;
    sf::Text text;
    sf::Font font;
    std::string fullText;
    size_t currentChar;
    float textSpeed; // Time per character
    sf::Clock textClock;
    bool isVisible;
    float wrapWidth;

public:
    TextBox(float width, float height);
    void setText(const std::string &newText);
    void update();
    void render(sf::RenderWindow &window, const sf::Vector2f &playerPos);
    void toggleVisibility();
    bool getVisibility() const;

    string wordWrap(const string &input, float width);
};

#endif // TEXTBOX_HPP
