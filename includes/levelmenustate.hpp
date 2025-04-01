#ifndef LEVEL_MENU
#define LEVEL_MENU

#include "gamestate.hpp"
#include "button.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include "gamestateman.hpp"

struct LevelButtons{
	float width , height;
	sf::Font font;
	sf::Text LevelName;
	sf::RectangleShape shape;
	bool isPressed;
	enum LevelButtonState{
		NotComplete = 0 , Complete
	};

	LevelButtonState state;

	LevelButtons(float posx , float posy , float w , float h , sf::Font* font , std::string&  levelName );

	void render(sf::RenderWindow& window , LevelButtons &lb) ;

	void updateCompletionStatusToComplete();
	void updateCompletionStatusToInComplete();

	void handleInput(sf::Event& event , sf::Vector2f& mousePos);


};

class LevelMenu : public GameState {
public:

	sf::Font font;
	sf::Text GameTitle;

	std::vector<Button> navigatorButtons;
	std::vector<LevelButtons> levelButtons;
	GameStateManager* stateManager;

	int currIndex = 0;

	LevelMenu(GameStateManager* stateManager , sf::RenderWindow& window);    
	void handleInput(sf::RenderWindow& window) override;
	void update(float deltaTime) override;
	void render(sf::RenderWindow& window) override;

};

#endif
