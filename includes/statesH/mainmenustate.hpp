#ifndef MAIN_MENU_STATE
#define MAIN_MENU_STATE

#include "gamestate.hpp"
#include "../componentsH/button.hpp"
#include "gamestateman.hpp"
#include <vector>


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

#endif 
