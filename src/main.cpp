#include "../includes/map.hpp"
#include "../includes/player.hpp"
#include "../includes/textBox.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>

using namespace sf;

void updateCamera(sf::View &view, const Player &player, const Map &map,
                  sf::RenderWindow &window) {
  sf::Vector2f playerPos = player.getPosition();
  sf::Vector2u windowSize = window.getSize();

  // Set a zoom factor (smaller value = more zoomed in)
  const float zoomFactor = 0.4f; // Adjust this value as needed

  // Apply zoom by resizing the view
  float viewWidth = windowSize.x * zoomFactor;
  float viewHeight = windowSize.y * zoomFactor;
  view.setSize(viewWidth, viewHeight);

  sf::Vector2f halfViewSize(viewWidth / 2.0f, viewHeight / 2.0f);
  sf::FloatRect mapBounds = map.getMapBounds();

  // Calculate view boundaries
  float leftBound = mapBounds.left + halfViewSize.x;
  float rightBound = mapBounds.left + mapBounds.width - halfViewSize.x;
  float topBound = mapBounds.top + halfViewSize.y;
  float bottomBound = mapBounds.top + mapBounds.height - halfViewSize.y;

  // If the map is smaller than the zoomed view in any dimension, center the
  // view on the map
  if (mapBounds.width < viewWidth) {
    leftBound = rightBound = mapBounds.left + mapBounds.width / 2.0f;
  }

  if (mapBounds.height < viewHeight) {
    topBound = bottomBound = mapBounds.top + mapBounds.height / 2.0f;
  }

  // Determine the new camera position (clamped to map boundaries)
  float newX = std::clamp(playerPos.x, leftBound, rightBound);
  float newY = std::clamp(playerPos.y, topBound, bottomBound);

  // Apply smooth camera movement
  float currentX = view.getCenter().x;
  float currentY = view.getCenter().y;
  const float cameraSmoothness = 0.1f;
  float smoothX = currentX + (newX - currentX) * cameraSmoothness;
  float smoothY = currentY + (newY - currentY) * cameraSmoothness;

  view.setCenter(smoothX, smoothY);
  window.setView(view);
}

int main() {
  RenderWindow window(VideoMode(1920, 1080), "Topdownwindow" , sf::Style::Fullscreen);
  window.setFramerateLimit(60);
  TextBox diag;
	std::vector<std::string> d = {
		"Hi i am gay jbikaskksksksksksksksksksksksksksksksksksksksksszmkmzkamzkmakmzakmzkmakmzakmazkmakzmakmzkamzkmakmzakmazkmakzmamkzm akmzkmamakmakma kmakmkamzkmmakzmakmakmakmakmzkamkmk sjwhjwhwjh" , "Hi i am valan" , "Hi i am Tamil Dasan" 
	};
  diag.loadDialogues(d);
  sf::View view(sf::FloatRect(0.f, 0.f, 1280.f, 720.f));

  // Create the map first
  Map map("assets/Home.tmx", "assets/HomeInteriors.png", 32.0f);

  // Initial render to calculate the proper scale
  window.clear();
  map.render(window);

  // Create player at the center of the map
  sf::FloatRect mapBounds = map.getMapBounds();
  float centerX = mapBounds.left + mapBounds.width / 2.0f;
  float centerY = mapBounds.top + mapBounds.height / 2.0f;
  Player player(centerX - 200, centerY + 100, 20.0f, 5.0f);

  // Scale the player to match the map's scale
  player.setScale(map.getScale());

  // Center the view on the player initially
  view.setCenter(player.getPosition());
  window.setView(view);

  // Initialize a clock for delta time calculation
  sf::Clock clock;

  // Track fullscreen state
  bool isFullscreen = false;

  while (window.isOpen()) {
    // Calculate delta time
    float deltaTime = clock.restart().asSeconds();

    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed)
        window.close();

      // Toggle fullscreen with F11
      if (event.type == Event::KeyPressed &&
          event.key.code == sf::Keyboard::F11) {
        isFullscreen = !isFullscreen;
        if (isFullscreen) {
          window.create(sf::VideoMode::getDesktopMode(), "Topdownwindow",
                        sf::Style::Fullscreen);
        } else {
          window.create(sf::VideoMode(1280, 720), "Topdownwindow",
                        sf::Style::Default);
        }
        window.setFramerateLimit(60);

        // Re-render map after window resize to recalculate scaling
        window.clear();
        map.render(window);

        // Update player scale to match new map scale
        player.setScale(map.getScale());
      }
    }

    // Update game logic
    player.update(map);
    updateCamera(view, player, map, window);

    // Render everything
    window.clear();
    map.render(window);
    player.draw(window); 
    diag.draw(window);


		diag.handleInput();
    // Display FPS (optional)
    static sf::Font font;
    static bool fontLoaded = false;
    if (!fontLoaded) {
      if (font.loadFromFile("assets/font.otf")) {
        fontLoaded = true;
      }
    }

    if (fontLoaded) {
      sf::Text fpsText;
      fpsText.setFont(font);
      fpsText.setCharacterSize(16);
      fpsText.setFillColor(sf::Color::White);
      fpsText.setString("FPS: " +
                        std::to_string(static_cast<int>(1.0f / deltaTime)));

      // Position the text relative to the view
      sf::Vector2f viewPos = window.getView().getCenter();
      sf::Vector2f viewSize = window.getView().getSize();
      fpsText.setPosition(viewPos.x - viewSize.x / 2 + 10,
                          viewPos.y - viewSize.y / 2 + 10);

      window.draw(fpsText);
    }

    window.display();
  }

  return 0;
}
