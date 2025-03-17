#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "../includes/map.hpp"

class Player {
public:
  Player(float x, float y, float size, float speed);
  void update(const Map &map);
  void draw(sf::RenderWindow &window);
  sf::FloatRect getBounds() const;
  sf::Vector2f getPosition() const;
  void setPosition(float x, float y);
  void setScale(float scale);

private:
  void handleInput(const Map &map);
  sf::RectangleShape shape;
  float speed;
};

#endif
