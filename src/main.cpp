#include "../includes/player.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

int main() {
  RenderWindow window(VideoMode(1280, 720), "Topdownwindow");
  window.setFramerateLimit(60);

  Player player(640.0f, 360.0f);

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed)
        window.close();
    }

    player.handleInput();

    window.clear();
    player.render(window);
    window.display();
  }

  return 0;
}
