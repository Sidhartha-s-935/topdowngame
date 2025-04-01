#include "../includes/gamestateman.hpp"
#include "../includes/mainmenustate.hpp"
#include "../includes/pausestate.hpp"
#include "../includes/levelmenustate.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>
#include <memory>

int main()
{
  // Create the main window
  sf::RenderWindow window(sf::VideoMode(1920, 1080), "Topdownwindow",sf::Style::Fullscreen);
  window.setFramerateLimit(60);

  // Create the game state manager
  GameStateManager stateManager;

  // Push initial state (main menu)
  /*stateManager.pushState(std::make_unique<MainMenuState>(&stateManager, window));*/

	stateManager.pushState(std::make_unique<LevelMenu>(&stateManager , window));


  // Initialize a clock for delta time calculation
  sf::Clock clock;


  // Main game loop
  while (window.isOpen() && !stateManager.isEmpty())
  {
    // Calculate delta time
    float deltaTime = clock.restart().asSeconds();

    // Get current state
    GameState *currentState = stateManager.getCurrentState();
    if (currentState == nullptr)
    {
      break;
    }

    // Handle input for current state
    currentState->handleInput(window);

    // Update current state
    currentState->update(deltaTime);

    if (auto pauseState = dynamic_cast<PauseState *>(currentState))
    {
      if (pauseState->hasPendingTransition())
      {
        Transition t = pauseState->getPendingTransition();
        // Clear the pending flag so we don’t re-process it later
        pauseState->clearPendingTransition();

        if (t == Transition::Resume)
        {
          // Resume: pop only the pause state.
          stateManager.popState();
        }
        else if (t == Transition::MainMenu)
        {
          // Main Menu: pop pause state and gameplay state, then push main menu.
          stateManager.popState(); // pop pause state
          stateManager.popState(); // pop gameplay state
          stateManager.pushState(std::make_unique<MainMenuState>(&stateManager, window));

          // Note: Adjust stateManager.getPointer() as needed to pass the stateManager pointer.
        }
        // After this, currentState may have changed.
        currentState = stateManager.getCurrentState();
      }
    }

    // Render the current state (using a default view for UI states if needed)
    currentState->render(window);

    window.display();
  }

  return 0;
}
