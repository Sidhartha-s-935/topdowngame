#ifndef GAME_STATE
#define GAME_STATE
 
#include <SFML/Graphics.hpp>
#include <memory>

class GameState {
public:
    virtual ~GameState() = default;
    virtual void handleInput(sf::RenderWindow& window) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};


#endif
