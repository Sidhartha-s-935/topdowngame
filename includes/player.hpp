#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
  Player(float startX, float startY);

  void handleInput();
  void update(float deltaTime);
  void render(sf::RenderWindow &window);

private:
  sf::RectangleShape shape;
  float speed;
};

#endif // PLAYER_HPP
