
#include "../includes/mainmenustate.hpp"
#include "../includes/gamestateman.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../includes/gameplaystate.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <memory>

MainMenuState::MainMenuState(GameStateManager* stateManeger , sf::RenderWindow& window) : stateManager(stateManeger) {
	/*this->stateManager = stateManager;*/

	if(this->font.loadFromFile("assets/font.otf")) {
		std::cerr << "Failed to load font! in main menu"  << '\n';
	}

	this->title.setFont(font);
	this->title.setFillColor(sf::Color::White);
	this->title.setCharacterSize(64);
	this->title.setString("GAME TITLE");

	sf::FloatRect titleRect = this->title.getLocalBounds();
	this->title.setOrigin(titleRect.left + titleRect.width / 2.0f , titleRect.top + titleRect.height / 2.0f);

	this->title.setPosition(window.getSize().x / 2.0f , window.getSize().y  / 4.0f);


	const float buttonWidth = 300.0f , buttonHeight = 60.0f , buttonY = window.getSize().y / 2.0f , spacing = 20.0f;

	std::string PlayGame = "PlayGame" , QuitGame = "Quit";

	this->buttons.emplace_back(
		window.getSize().x / 2.0f - buttonWidth / 2.0f , buttonY , buttonWidth , buttonHeight , 
		&this->font , PlayGame , sf::Color(100 , 100 , 100) , sf::Color(150 , 150 , 150) , 
		sf::Color(70 , 70 ,70) , sf::Color::White , [this , &window] () {
			this->stateManager->pushState(std::make_unique<GameplayState>(this->stateManager , window));
		}
	);

	this->buttons.emplace_back( window.getSize().x / 2.0f - buttonWidth / 2.0f , buttonY + buttonHeight + spacing , buttonWidth , buttonHeight , &font , QuitGame , sf::Color(100 , 100 , 100) , sf::Color(150 , 150 , 150) , sf::Color(70 , 70 , 70) , sf::Color::White , [&window]() {
		window.close();
	}  );
}

void MainMenuState::handleInput(sf::RenderWindow & window) {
	sf::Event e;
	while(window.pollEvent(e)) {
		if(e.type == sf::Event::Closed) window.close();
	}

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	for(auto& it : this->buttons) {
		it.handleEvent(e, mousePos);
	}
}

void MainMenuState::update(float deltaTime) {
	sf::Vector2i mousePos = sf::Mouse::getPosition();


	for(auto& it : this->buttons) {
		it.update(sf::Vector2f(mousePos.x , mousePos.y));
	}
}

void MainMenuState::render(sf::RenderWindow & window) {
	window.clear(sf::Color(30 , 30 , 30));

	window.draw(this->title);

	for(auto& it : this->buttons) {
		it.render(window);
	}
}


