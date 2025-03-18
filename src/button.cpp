#include "../includes/button.hpp"

Button::Button(float x, float y, float width, float height, sf::Font *font,
               const std::string &text, const sf::Color &idleColor,
               const sf::Color &hoverColor, const sf::Color &activeColor,
               const sf::Color &textColor, std::function<void()> callback)
    : font(font), idleColor(idleColor), hoverColor(hoverColor),
      activeColor(activeColor), textColor(textColor), isPressed(false),
      isHovered(false), callback(callback) {

  shape.setPosition(sf::Vector2f(x, y));
  shape.setSize(sf::Vector2f(width, height));
  shape.setFillColor(idleColor);

  this->text.setFont(*font);
  this->text.setString(text);
  this->text.setFillColor(textColor);
  this->text.setCharacterSize(24);

  // Center the text on the button
  sf::FloatRect textRect = this->text.getLocalBounds();
  this->text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
  this->text.setPosition(x + width / 2.0f, y + height / 2.0f);
}

void Button::update(const sf::Vector2f &mousePos) {
  // Idle by default
  shape.setFillColor(idleColor);
  isHovered = false;

  // Check if mouse is hovering over the button
  if (shape.getGlobalBounds().contains(mousePos)) {
    isHovered = true;

    // Hovering
    shape.setFillColor(hoverColor);

    // Clicked
    if (isPressed) {
      shape.setFillColor(activeColor);
    }
  }
}

void Button::handleEvent(const sf::Event &event, const sf::Vector2f &mousePos) {
  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      if (shape.getGlobalBounds().contains(mousePos)) {
        isPressed = true;
      }
    }
  }

  if (event.type == sf::Event::MouseButtonReleased) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      if (isPressed && shape.getGlobalBounds().contains(mousePos)) {
        // Execute callback function
        callback();
      }
      isPressed = false;
    }
  }
}

void Button::render(sf::RenderWindow &window) {
  window.draw(shape);
  window.draw(text);
}
