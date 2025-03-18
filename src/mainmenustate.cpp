// mainmenustate.cpp - Fixed implementation
#include "../includes/mainmenustate.hpp"
#include "../includes/gameplaystate.hpp"
#include "../includes/gamestateman.hpp"
#include <iostream>

MainMenuState::MainMenuState(GameStateManager *stateManager,
                             sf::RenderWindow &window)
    : stateManager(stateManager) {

  // Load font
  if (!font.loadFromFile("assets/font.otf")) {
    std::cerr << "Failed to load font!" << std::endl;
  }

  // Setup title
  title.setFont(font);
  title.setString("GAME TITLE");
  title.setCharacterSize(64);
  title.setFillColor(sf::Color::White);

  // Center the title
  sf::FloatRect titleRect = title.getLocalBounds();
  title.setOrigin(titleRect.left + titleRect.width / 2.0f,
                  titleRect.top + titleRect.height / 2.0f);
  title.setPosition(window.getSize().x / 2.0f, window.getSize().y / 4.0f);

  // Create buttons
  float buttonWidth = 300.0f;
  float buttonHeight = 60.0f;
  float buttonY = window.getSize().y / 2.0f;
  float spacing = 20.0f;

  // Play button - Fixed with explicit lambda capture
  buttons.emplace_back(
      window.getSize().x / 2.0f - buttonWidth / 2.0f, buttonY, buttonWidth,
      buttonHeight, &font, "Play Game", sf::Color(100, 100, 100), // Idle color
      sf::Color(150, 150, 150),                                   // Hover color
      sf::Color(70, 70, 70), // Active color
      sf::Color::White,      // Text color
      [this, &window]() {
        this->stateManager->pushState(
            std::make_unique<GameplayState>(this->stateManager, window));
      });

  // Quit button
  buttons.emplace_back(window.getSize().x / 2.0f - buttonWidth / 2.0f,
                       buttonY + buttonHeight + spacing, buttonWidth,
                       buttonHeight, &font, "Quit",
                       sf::Color(100, 100, 100), // Idle color
                       sf::Color(150, 150, 150), // Hover color
                       sf::Color(70, 70, 70),    // Active color
                       sf::Color::White,         // Text color
                       [&window]() { window.close(); });
}

void MainMenuState::handleInput(sf::RenderWindow &window) {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }

    // Check button events
    sf::Vector2f mousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (auto &button : buttons) {
      button.handleEvent(event, mousePos);
    }
  }
}

void MainMenuState::update(float deltaTime) {
  sf::Vector2i mousePos = sf::Mouse::getPosition();

  // Update buttons
  for (auto &button : buttons) {
    button.update(sf::Vector2f(mousePos.x, mousePos.y));
  }
}

void MainMenuState::render(sf::RenderWindow &window) {
  window.clear(sf::Color(30, 30, 30)); // Dark background for menu

  // Draw title
  window.draw(title);

  // Draw buttons
  for (auto &button : buttons) {
    button.render(window);
  }
}
