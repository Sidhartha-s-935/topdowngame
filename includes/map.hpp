#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Map {
public:
  Map(const std::string &tmxFilePath, const std::string &tilesetPath,
      float tileSize);
  void render(sf::RenderWindow &window);
  bool isColliding(const sf::FloatRect &playerBounds) const;
  void scaleToFit(const sf::RenderWindow &window);
  sf::FloatRect getMapBounds() const;

private:
  sf::Texture tilesetTexture;
  sf::VertexArray vertices;
  std::vector<sf::FloatRect> collisionRects;
  float tileSize;
};

#endif // MAP_HPP
