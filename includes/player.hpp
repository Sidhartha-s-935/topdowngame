#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "map.hpp"
#include <SFML/Graphics.hpp>

class Player {
private:
  sf::RectangleShape shape;
  float speed;

public:
  Player(float x, float y, float size, float speed);
  void handleInput(const Map &map);
  void update(const Map &map);
  void draw(sf::RenderWindow &window);
  sf::Vector2f getPosition() const;

  // Add this function to return the bounding box of the player
  sf::FloatRect getBounds() const;
};

#endif
