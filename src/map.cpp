#include "../includes/map.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ostream>
#include <sstream>
#include <tinyxml2.h>

using namespace tinyxml2;
using namespace std;

Map::Map(const std::string &tmxFilePath, const std::string &tilesetPath,
         float tileSize)
    : tileSize(tileSize) {

  if (!tilesetTexture.loadFromFile(tilesetPath)) {
    std::cerr << "Failed to load tileset texture!" << std::endl;
    return;
  } else {
    std::cout << "success tilesetTexture" << std::endl;
  }

  XMLDocument doc;
  if (doc.LoadFile(tmxFilePath.c_str()) != XML_SUCCESS) {
    std::cerr << "Failed to load map file!" << std::endl;
    return;
  }

  XMLElement *mapElement = doc.FirstChildElement("map");
  int width = mapElement->IntAttribute("width");
  int height = mapElement->IntAttribute("height");

  vertices.setPrimitiveType(sf::Quads);
  int totalTiles = width * height;
  vertices.resize(totalTiles * 4);

  int tileCounter = 0;

  for (XMLElement *layerElement = mapElement->FirstChildElement("layer");
       layerElement != nullptr;
       layerElement = layerElement->NextSiblingElement("layer")) {

    std::string layerName = layerElement->Attribute("name");
    bool isCollidable =
        (layerName == "bounding walls" || layerName == "Floor objects above" ||
         layerName == "Floor objects" || layerName == "wall object above" ||
         layerName == " wall objects" || layerName == "inner walls");

    XMLElement *dataElement = layerElement->FirstChildElement("data");
    const char *encodedData = dataElement->GetText();

    if (!encodedData)
      continue;

    std::vector<unsigned int> tileIndices;
    std::stringstream ss(encodedData);
    std::string tile;

    while (std::getline(ss, tile, ',')) {
      tileIndices.push_back(std::stoi(tile));
    }

    for (size_t i = 0; i < tileIndices.size(); ++i) {
      unsigned int tileID = tileIndices[i];

      if (tileID == 0)
        continue; // Empty tile, skip rendering

      int tu = (tileID - 1) %
               (tilesetTexture.getSize().x / static_cast<int>(tileSize));
      int tv = (tileID - 1) /
               (tilesetTexture.getSize().x / static_cast<int>(tileSize));

      int x = (i % width) * tileSize;
      int y = (i / width) * tileSize;

      if (tileCounter * 4 >= vertices.getVertexCount()) {
        vertices.resize(vertices.getVertexCount() + 4);
      }

      sf::Vertex *quad = &vertices[tileCounter * 4];

      // Define the 4 corners of the tile quad
      quad[0].position = sf::Vector2f(x, y);
      quad[1].position = sf::Vector2f(x + tileSize, y);
      quad[2].position = sf::Vector2f(x + tileSize, y + tileSize);
      quad[3].position = sf::Vector2f(x, y + tileSize);

      // Define the 4 texture coordinates
      quad[0].texCoords = sf::Vector2f(tu * tileSize, tv * tileSize);
      quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);
      quad[2].texCoords =
          sf::Vector2f((tu + 1) * tileSize, (tv + 1) * tileSize);
      quad[3].texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);

      if (isCollidable) {
        // Store the collision rectangle
        collisionRects.emplace_back(x, y, tileSize, tileSize);
      }

      tileCounter++;
    }
  }
}

void Map::scaleToFit(const sf::RenderWindow &window) {
  sf::Vector2u windowSize = window.getSize();
  sf::FloatRect mapBounds(0.f, 0.f, vertices.getBounds().width,
                          vertices.getBounds().height);

  float scaleX = static_cast<float>(windowSize.x) / mapBounds.width;
  float scaleY = static_cast<float>(windowSize.y) / mapBounds.height;

  float scale = std::min(scaleX, scaleY);

  for (size_t i = 0; i < vertices.getVertexCount(); ++i) {
    vertices[i].position *= scale;
  }
}

void Map::render(sf::RenderWindow &window) {
  scaleToFit(window);

  sf::RenderStates states;
  states.texture = &tilesetTexture;
  window.draw(vertices, states);
}

bool Map::isColliding(const sf::FloatRect &playerBounds) const {
  for (const auto &rect : collisionRects) {
    if (rect.intersects(playerBounds))
      return true;
  }
  return false;
}

sf::FloatRect Map::getMapBounds() const {
  float mapWidth = vertices.getBounds().width;
  float mapHeight = vertices.getBounds().height;
  return sf::FloatRect(0.f, 0.f, mapWidth, mapHeight);
}
