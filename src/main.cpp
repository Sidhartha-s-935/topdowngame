#include "../includes/statesH/gamestateman.hpp"
#include "../includes/statesH/pausestate.hpp"
#include "../includes/statesH/mainmenustate.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <memory>

int main() {

	sf::RenderWindow window(sf::VideoMode(1920 , 1080) , "REWIND" , sf::Style::Fullscreen);

	window.setFramerateLimit(60);

	GameStateManager stateManager;

	stateManager.pushState(std::make_unique<MainMenuState>(&stateManager , window));

	sf::Clock clock;

	while(window.isOpen() && !stateManager.isEmpty()) {


		float deltaTime = clock.restart().asSeconds();
		GameState* currentState = stateManager.getCurrentState();

		if(currentState == nullptr) break;

		currentState->handleInput(window);
		currentState->update(deltaTime);

		if(auto pauseState = dynamic_cast<PauseState*>(currentState)) {
			if(pauseState->hasPendingTransition()) {
				Transition t = pauseState->getPendingTransition();
				pauseState->clearPendingTransition();

				if(t == Transition::Resume) {
					stateManager.popState();
				}

				else if(t == Transition::MainMenu) {
					stateManager.popState();
					stateManager.popState();
					stateManager.pushState(std::make_unique<MainMenuState>(&stateManager , window));
				}

				currentState = stateManager.getCurrentState();
			}	
		}

		currentState->render(window);

		window.display();
	}

	return 0;

	
}

