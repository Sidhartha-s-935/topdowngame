#include "../includes/textbox.hpp"
#include <sstream>

TextBox::TextBox(float width, float height) 
    : currentChar(0), textSpeed(0.05f), isVisible(false) {

    // Load a font (Make sure "assets/font.otf" exists or change the path)
    if (!font.loadFromFile("assets/font.otf")) {
        throw std::runtime_error("Failed to load font");
    }

    // Configure the text
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    // Configure the text box background
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(0, 0, 0, 200)); // Semi-transparent black
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(2);

    // Set default word wrap width
    wrapWidth = width - 20; // Add padding to avoid touching edges
}

void TextBox::setText(const std::string &newText) {
    fullText = wordWrap(newText, wrapWidth);
    currentChar = 0; // Reset text animation
    text.setString(""); // Clear current text
    textClock.restart(); // Restart timer
}

void TextBox::update() {
    if (isVisible && currentChar < fullText.size()) {
        if (textClock.getElapsedTime().asSeconds() >= textSpeed) {
            currentChar++;
            text.setString(fullText.substr(0, currentChar)); // Gradually reveal text
            textClock.restart();
        }
    }
}

void TextBox::render(sf::RenderWindow &window, const sf::Vector2f &playerPos) {
    if (isVisible) {
        // Position the text box under the player
        sf::Vector2f boxPos(playerPos.x - background.getSize().x / 2, playerPos.y + 40);
        background.setPosition(boxPos);
        text.setPosition(boxPos.x + 10, boxPos.y + 10);

        window.draw(background);
        window.draw(text);
    }
}

void TextBox::toggleVisibility() {
    isVisible = !isVisible;
    if (!isVisible) {
        text.setString(""); // Clear text when hidden
        currentChar = 0;
    }
}

bool TextBox::getVisibility() const {
    return isVisible;
}

// Word wrapping function
std::string TextBox::wordWrap(const std::string &input, float width) {
    std::istringstream words(input);
    std::string word, wrappedText, line;
    
    while (words >> word) {
        std::string tempLine = line.empty() ? word : line + " " + word;

        // Check if the new line width exceeds the box width
        text.setString(tempLine);
        if (text.getLocalBounds().width > width) {
            wrappedText += line + "\n"; // Add current line to output
            line = word; // Start a new line
        } else {
            line = tempLine;
        }
    }

    wrappedText += line; // Add the last line
    return wrappedText;
}
