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
  sf::FloatRect getMapBounds() const;
  float getScale() const;

private:
  void scaleToFit(const sf::RenderWindow &window);
  
  float tileSize;
  float scale;
  sf::Texture tilesetTexture;
  sf::VertexArray vertices;
  sf::VertexArray scaledVerticesForRendering;
  std::vector<sf::FloatRect> originalCollisionRects;
  std::vector<sf::FloatRect> collisionRects;
};

#endif
