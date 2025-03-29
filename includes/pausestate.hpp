#ifndef PAUSE_STATE 

#define PAUSE_STATE
#include "gamestate.hpp"
#include "button.hpp"
#include <vector>
#include "../includes/gamestateman.hpp"

enum class Transition
{
    None,
    Resume,
    MainMenu
};

class PauseState : public GameState
{
private:
    sf::Font font;
    sf::Text pauseText;
    sf::RectangleShape background;
    std::vector<Button> buttons;
    GameStateManager *stateManager;
    Transition pendingTransition = Transition::None;

public:
    PauseState(GameStateManager *stateManager, sf::RenderWindow &window);
    Transition getPendingTransition() const { return pendingTransition; }
    void clearPendingTransition() { pendingTransition = Transition::None; }
    // You might want to also add a flag helper:
    bool hasPendingTransition() const { return pendingTransition != Transition::None; }
    sf::RenderWindow &window;
    void handleInput(sf::RenderWindow &window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;
};



#endif // !
