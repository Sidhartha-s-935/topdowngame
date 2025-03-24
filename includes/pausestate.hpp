#ifndef PAUSE_STATE 

#define PAUSE_STATE
#include "gamestate.hpp"
#include "button.hpp"
#include <vector>
#include "../includes/gamestateman.hpp"

class PauseState : public GameState {
private:
    sf::Font font;
    sf::Text pauseText;
    sf::RectangleShape background;
    std::vector<Button> buttons;
    GameStateManager* stateManager;
    
public:
    PauseState(GameStateManager* stateManager, sf::RenderWindow& window);
    
    void handleInput(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
};



#endif // !
