#pragma once
#include "gamestate.hpp"
#include "map.hpp"
#include "player.hpp"
#include <SFML/Graphics.hpp>

class GameStateManager;

class GameplayState : public GameState {
private:
    Map map;
    Player player;
    sf::View gameView;
    sf::Font font;
    GameStateManager* stateManager;
    sf::RenderWindow& window; // Add reference to window
    
    void updateCamera();
    
public:
    GameplayState(GameStateManager* stateManager, sf::RenderWindow& window);
    
    void handleInput(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    
    // Method to handle pause input
    void checkForPause();
};
