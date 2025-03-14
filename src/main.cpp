#include "../includes/map.hpp"
#include "../includes/player.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

void updateCamera(sf::View &view, const Player &player, const Map &map,
                  sf::RenderWindow &window) {
  const float margin = 0.f;

  sf::Vector2f playerPos = player.getPosition();
  sf::Vector2u windowSize = window.getSize();
  sf::Vector2f halfWindowSize(windowSize.x / 2.0f, windowSize.y / 2.0f);

  sf::FloatRect mapBounds =
      map.getMapBounds(); // We'll implement this function in the Map class

  float leftBound = mapBounds.left + halfWindowSize.x;
  float rightBound = mapBounds.left + mapBounds.width - halfWindowSize.x;
  float topBound = mapBounds.top + halfWindowSize.y;
  float bottomBound = mapBounds.top + mapBounds.height - halfWindowSize.y;

  float newX = std::clamp(playerPos.x, leftBound, rightBound);
  float newY = std::clamp(playerPos.y, topBound, bottomBound);

  view.setCenter(newX, newY);
  window.setView(view);
}

int main() {
  RenderWindow window(VideoMode(1280, 720), "Topdownwindow");
  window.setFramerateLimit(60);
  sf::View view(sf::FloatRect(0.f, 0.f, 1280.f, 720.f));

  Player player(640.0f, 390.0f, 20.0f,
                10.f); // Spawn player in the middle of the screen
  Map map("assets/Home.tmx", "assets/HomeInteriors.png",
          32.0f); // Replace paths with your files

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed)
        window.close();
    }

    player.update(map);

    // Collision Check
    if (map.isColliding(player.getBounds())) {
      std::cout << "Collision detected!" << std::endl;
    }
    updateCamera(view, player, map, window);
    window.clear();
    map.render(window);
    player.draw(window);
    window.display();
  }

  return 0;
}
