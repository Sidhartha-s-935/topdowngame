#include "../../includes/statesH/gameplaystate.hpp"
#include "../../includes/statesH/gamestate.hpp"
#include "../../includes/statesH/pausestate.hpp"
#include <algorithm> // For std::clamp
#include <cmath>
#include <iostream>

GameplayState::GameplayState(GameStateManager *stateManager,
                             sf::RenderWindow &window)
    : stateManager(stateManager),
      map("assets/Home.tmx", "assets/HomeInteriors.png", 32.0f), window(window),
      player(0.0f, 0.0f, 20.0f, 5.0f) { // Initialize player with default values

  // Initialize view
  gameView =
      sf::View(sf::FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y));

  // Load font
  if (!font.loadFromFile("assets/font.otf")) {
    std::cerr << "Failed to load font!" << std::endl;
  }

  // Initialize player at the center of the map
  sf::FloatRect mapBounds = map.getMapBounds();
  float centerX = mapBounds.left + mapBounds.width / 2.0f;
  float centerY = mapBounds.top + mapBounds.height / 2.0f;

  // Set player position instead of creating a new player
  player = Player(centerX, centerY + 100, 20.0f, 5.0f);

  // Scale the player to match the map's scale
  player.setScale(map.getScale());

  // Center the view on the player initially
  gameView.setCenter(player.getPosition());
  window.setView(gameView);
}

void GameplayState::updateCamera() {
  sf::Vector2f playerPos = player.getPosition();
  sf::Vector2u windowSize = window.getSize();

  // Set a zoom factor (smaller value = more zoomed in)
  const float zoomFactor = 0.4f; // Adjust this value as needed

  // Apply zoom by resizing the view
  float viewWidth = windowSize.x * zoomFactor;
  float viewHeight = windowSize.y * zoomFactor;
  gameView.setSize(viewWidth, viewHeight);

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
  float currentX = gameView.getCenter().x;
  float currentY = gameView.getCenter().y;
  const float cameraSmoothness = 0.1f;
  float smoothX = currentX + (newX - currentX) * cameraSmoothness;
  float smoothY = currentY + (newY - currentY) * cameraSmoothness;

  gameView.setCenter(smoothX, smoothY);
  window.setView(gameView);
}

void GameplayState::checkForPause() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
    stateManager->pushState(std::make_unique<PauseState>(stateManager, window));
  }
}

void GameplayState::handleInput(sf::RenderWindow &window) {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }

    // Handle fullscreen toggle
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::F11) {
      static bool isFullscreen = false;
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

    // Check for pause
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {
      stateManager->pushState(
          std::make_unique<PauseState>(stateManager, window));
      return;
    }
  }

  // Handle player input
  player.handleInput(map);
}

void GameplayState::update(float deltaTime) {
  // Update camera
  updateCamera();
}

void GameplayState::render(sf::RenderWindow &window) {
  window.setView(gameView);
  window.clear();

  // Draw the map
  map.render(window);

  // Draw the player
  player.draw(window);

  // Display FPS
  static sf::Clock fpsClock;
  static float fpsUpdateTime = 0.0f;
  static int frameCount = 0;
  static float currentFps = 0.0f;

  frameCount++;
  fpsUpdateTime += fpsClock.restart().asSeconds();

  if (fpsUpdateTime >= 0.5f) { // Update FPS every half second
    currentFps = frameCount / fpsUpdateTime;
    frameCount = 0;
    fpsUpdateTime = 0.0f;
  }

  sf::Text fpsText;
  fpsText.setFont(font);
  fpsText.setCharacterSize(16);
  fpsText.setFillColor(sf::Color::White);
  fpsText.setString("FPS: " + std::to_string(static_cast<int>(currentFps)));

  // Position the text relative to the view
  sf::Vector2f viewPos = window.getView().getCenter();
  sf::Vector2f viewSize = window.getView().getSize();
  fpsText.setPosition(viewPos.x - viewSize.x / 2 + 10,
                      viewPos.y - viewSize.y / 2 + 10);

  window.draw(fpsText);
}
