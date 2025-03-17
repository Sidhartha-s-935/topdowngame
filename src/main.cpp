#include "../includes/map.hpp"
#include "../includes/player.hpp"
#include "../includes/textbox.hpp"
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

    Player player(640.0f, 390.0f, 20.0f, 10.f);
    Map map("assets/Home.tmx", "assets/HomeInteriors.png", 32.0f);
    TextBox textBox(400, 100); // Create text box

    bool eKeyPressed = false; // To prevent multiple triggers

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::E) {
                if (!eKeyPressed) { // Prevent repeated toggling from key hold
                    textBox.toggleVisibility();
                    if (textBox.getVisibility()) {
                        textBox.setText("Hello, traveler! Welcome to this world.HIabfouawboabjbnvbsojvbnabeojbozbxjovbabouva");
                    }
                }
                eKeyPressed = true;
            }

            if (event.type == Event::KeyReleased && event.key.code == Keyboard::E) {
                eKeyPressed = false;
            }
        }

        player.update(map);
        textBox.update();

        window.clear();
        map.render(window);
        player.draw(window);
        textBox.render(window, player.getPosition()); // Render text box
        window.display();
    }
}
