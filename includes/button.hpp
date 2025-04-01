#ifndef BUTTON

#define BUTTON


#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
private:
  sf::RectangleShape shape;
  sf::Text text;
  sf::Font *font;
  sf::Color idleColor;
  sf::Color hoverColor;
  sf::Color activeColor;
  sf::Color textColor;

  bool isPressed;
  bool isHovered;

  std::function<void()> callback;

public:
  Button(float x, float y, float width, float height, sf::Font *font, const char* text, const sf::Color &idleColor,
         const sf::Color &hoverColor, const sf::Color &activeColor,
         const sf::Color &textColor, std::function<void()> callback);

  void update(const sf::Vector2f &mousePos);
  void render(sf::RenderWindow &window);
  void handleEvent(const sf::Event &event, const sf::Vector2f &mousePos);
};


#endif // !BUTTON
