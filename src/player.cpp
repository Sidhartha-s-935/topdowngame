#include "../includes/player.hpp"
#include <iostream>

Player::Player(float x, float y, float size, float speed) : speed(speed) {
  shape.setSize(sf::Vector2f(size, size));
  shape.setFillColor(sf::Color::Green);
  shape.setPosition(x, y);
}

void Player::handleInput(const Map &map) {
  sf::Vector2f originalPosition = shape.getPosition();
  sf::Vector2f movement(0.f, 0.f);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    movement.y -= speed;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    movement.y += speed;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    movement.x -= speed;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    movement.x += speed;

  shape.move(movement);

  if (map.isColliding(shape.getGlobalBounds())) {
    std::cout << "Collision Detected\n";
    shape.setPosition(originalPosition); // Revert movement
  }
}

void Player::update(const Map &map) { handleInput(map); }

void Player::draw(sf::RenderWindow &window) { window.draw(shape); }

sf::FloatRect Player::getBounds() const { return shape.getGlobalBounds(); }

sf::Vector2f Player::getPosition() const { return shape.getPosition(); }
