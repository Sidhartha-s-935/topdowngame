#include "../includes/gamestateman.hpp"
#include "../includes/mainmenustate.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>

int main() {
  // Create the main window
  sf::RenderWindow window(sf::VideoMode(1920, 1080), "Topdownwindow",
                          sf::Style::Fullscreen);
  window.setFramerateLimit(60);

  // Create the game state manager
  GameStateManager stateManager;

  // Push initial state (main menu)
  stateManager.pushState(
      std::make_unique<MainMenuState>(&stateManager, window));

  // Initialize a clock for delta time calculation
  sf::Clock clock;

  // Main game loop
  while (window.isOpen() && !stateManager.isEmpty()) {
    // Calculate delta time
    float deltaTime = clock.restart().asSeconds();

    // Get current state
    GameState *currentState = stateManager.getCurrentState();
    if (currentState == nullptr) {
      break;
    }

    // Handle input for current state
    currentState->handleInput(window);

    // Update current state
    currentState->update(deltaTime);

    // Render current state
    window.clear();
    currentState->render(window);
    window.display();
  }

  return 0;
}
