#include "../includes/player.hpp"
#include <cmath>
#include <iostream>

Player::Player(float x, float y, float size, float speed) : speed(speed) {
  shape.setSize(sf::Vector2f(size, size));
  shape.setFillColor(sf::Color::Green);
  shape.setPosition(x, y);

  // Center the origin of the shape for smoother movement
  shape.setOrigin(size / 2.0f, size / 2.0f);
}

void Player::handleInput(const Map &map) {
  sf::Vector2f originalPosition = shape.getPosition();
  sf::Vector2f movement(0.f, 0.f);

  // Detect diagonal movement
  bool movingDiagonally = false;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    movement.y -= speed;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    movement.y += speed;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    movement.x -= speed;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    movement.x += speed;

  // Check if moving diagonally
  if (movement.x != 0 && movement.y != 0) {
    movingDiagonally = true;
    // Normalize diagonal movement to prevent faster diagonal movement
    float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    movement.x = movement.x / length * speed;
    movement.y = movement.y / length * speed;
  }

  // Try horizontal movement first
  if (movement.x != 0) {
    shape.move(movement.x, 0);
    if (map.isColliding(shape.getGlobalBounds())) {
      shape.setPosition(originalPosition.x, shape.getPosition().y);
    }
  }

  // Then try vertical movement
  if (movement.y != 0) {
    shape.move(0, movement.y);
    if (map.isColliding(shape.getGlobalBounds())) {
      shape.setPosition(shape.getPosition().x, originalPosition.y);
    }
  }
}

void Player::update(const Map &map) { handleInput(map); }

void Player::draw(sf::RenderWindow &window) { window.draw(shape); }

sf::FloatRect Player::getBounds() const { return shape.getGlobalBounds(); }

sf::Vector2f Player::getPosition() const { return shape.getPosition(); }

void Player::setPosition(float x, float y) { shape.setPosition(x, y); }

// Scale the player size according to the map scale
void Player::setScale(float scale) {
  // Preserve the player's original size ratio
  float currentSize = shape.getSize().x;
  // shape.setSize(sf::Vector2f(currentSize * scale, currentSize * scale));
  // shape.setOrigin(currentSize * scale / 2.0f, currentSize * scale / 2.0f);
}
