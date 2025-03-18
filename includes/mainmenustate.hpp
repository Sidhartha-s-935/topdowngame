#pragma once
#include "gamestate.hpp"
#include "button.hpp"
#include <vector>

class GameStateManager;
class GameplayState; // Forward declaration

class MainMenuState : public GameState {
private:
    sf::Font font;
    sf::Text title;
    std::vector<Button> buttons;
    GameStateManager* stateManager;
    
public:
    MainMenuState(GameStateManager* stateManager, sf::RenderWindow& window);
    
    void handleInput(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
};
