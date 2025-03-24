#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <memory>
#include "../includes/pausestate.hpp"
#include "../includes/mainmenustate.hpp"


PauseState::PauseState(GameStateManager* stateManager , sf::RenderWindow & window) {
	this->stateManager = stateManager;

	if(this->font.loadFromFile("assets/font.otf")) {
		std::cerr << "failed to load font for pause menu" << '\n';
	}

	std::string PauseGame = "PAUSE" , ExitGame = "EXIT GAME" , MainMenu = "MAIN MENU" , Resume = "RESEUME";

	this->pauseText.setFont(this->font);
	this->pauseText.setCharacterSize(64);
	this->pauseText.setFillColor(sf::Color::White);
	this->pauseText.setString(PauseGame);

	sf::FloatRect textRect = this->pauseText.getLocalBounds();

	this->pauseText.setOrigin(textRect.left + textRect.width / 2.0f , textRect.top + textRect.height / 2.0f);

	this->pauseText.setPosition(window.getSize().x / 2.0f , window.getSize().y / 4.0f);


	this->background.setSize(sf::Vector2f(window.getSize()));

	this->background.setFillColor(sf::Color(210 , 140 , 105 , 255));


	const float  buttonWidth = 300.f , buttonHeight = 60.0f , buttonY = window.getSize().y / 2.0f , spacing = 20.0f ;
	const float leftOffset = window.getSize().x / 2.0f - buttonWidth / 2.0f;

	this->buttons.emplace_back(window.getSize().x / 2.0f - buttonWidth / 2.0f , 
			    buttonY , buttonWidth , buttonHeight , 
			    &this->font , Resume , 
			    sf::Color(100 , 100 , 100) , sf::Color(150 , 150 , 150) , sf::Color(70,70 , 70) , sf::Color::White , 
			    [this]() {
				this->stateManager->print();
				if(!this->stateManager->isEmpty()) this->stateManager->popState();

				} 
	);

	this->buttons.emplace_back(window.getSize().x / 2.0f - buttonWidth / 2.0f , 
			    buttonY + buttonHeight + spacing , buttonWidth , buttonHeight , 
			    &this->font , MainMenu , 
			    sf::Color(100 , 100 , 100) ,sf::Color(150 , 150 , 150) , sf::Color(70 , 70 , 70) , sf::Color::White , 
			    [this , &window]() {
				while(!this->stateManager->isEmpty()) {
					this->stateManager->popState();
				}
				this->stateManager->pushState(std::make_unique<MainMenuState>(this->stateManager , window));
			    } 
	);


	this->buttons.emplace_back(window.getSize().x / 2.0f - buttonWidth / 2.0f , 
			    buttonY + 2 * (buttonHeight + spacing) , buttonWidth , buttonHeight , 
			    &this->font , ExitGame , 
			    sf::Color(100 , 100 , 100) , sf::Color(150 , 150 , 150) , sf::Color(70 , 70 , 70) , sf::Color::White , 
			    [&window]() {
				window.close();
	});
}

void PauseState::handleInput(sf::RenderWindow& window) {
	sf::Event e;
	while(window.pollEvent(e)) {
		if(e.type == sf::Event::Closed) {
			window.close();
		}
	

		if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) {
			if(!this->stateManager->isEmpty()) {
				this->stateManager->popState();
				return;
			}

		}

		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		for(auto& it : this->buttons) {
			it.handleEvent(e , mousePos);
		}
	}
}

void PauseState::update(float deltaTime) {
	sf::Vector2i mousePos = sf::Mouse::getPosition();

	for(auto& it : this->buttons) {
		it.update(sf::Vector2f(mousePos.x , mousePos.y));
	}
}

void PauseState::render(sf::RenderWindow& window) {
	window.setView(window.getDefaultView());

	window.draw(background);
	window.draw(pauseText);

	for(auto& it : this->buttons) {
		it.render(window);
	}
}



