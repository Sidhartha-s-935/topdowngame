#include "../includes/player.hpp"

Player::Player(float startX, float startY) : speed(300.0f) {
  shape.setSize(sf::Vector2f(50.0f, 50.0f));
  shape.setFillColor(sf::Color::Green);
  shape.setPosition(startX, startY);
}

void Player::handleInput() {
  sf::Vector2f movement(0.0f, 0.0f);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    movement.y -= speed;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    movement.y += speed;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    movement.x -= speed;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    movement.x += speed;

  shape.move(movement * (1.0f / 60.0f));
}

void Player::update(float deltaTime) {}

void Player::render(sf::RenderWindow &window) { window.draw(shape); }
