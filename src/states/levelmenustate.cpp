#include "../../includes/statesH/levelmenustate.hpp"
#include "../../includes/statesH/gameplaystate.hpp"
#include "../../includes/statesH/mainmenustate.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <memory>
#include <string>

			   
LevelButtons::LevelButtons(GameStateManager* stateManager , float posx  , float posy ,  float w ,float h , sf::Font* font , std::string &levelName){
	this->stateManager = stateManager;
	this->width = w;
	this->height = h;
	this->shape.setPosition(sf::Vector2f(posx , posy));
	this->shape.setSize(sf::Vector2f(w , h));
	this->shape.setFillColor(sf::Color::White);

	this->state = this->LevelButtonState::NotComplete; 

	this->LevelName.setFont(*font);
	this->LevelName.setCharacterSize(48);
	this->LevelName.setString(levelName);
	this->LevelName.setFillColor(sf::Color::Blue);	

	sf::FloatRect LevelNameRect = this->LevelName.getLocalBounds();

	this->LevelName.setOrigin(LevelNameRect.left + LevelNameRect.width / 2.0f , LevelNameRect.top + LevelNameRect.height / 2.0f);
	this->LevelName.setPosition(posx + w / 2.0f , posy + h / 2.0f);
};

void LevelButtons::handleInput(sf::Event& event , sf::Vector2f& mousePos , sf::RenderWindow& window) {
	if(event.type == sf::Event::MouseButtonPressed) {
		if(event.mouseButton.button == sf::Mouse::Left) {
			if(this->shape.getGlobalBounds().contains(mousePos)) this->isPressed = true;
		}
	}

	if(event.type == sf::Event::MouseButtonReleased) {
		if(event.mouseButton.button == sf::Mouse::Left) {
			if(this->isPressed && this->shape.getGlobalBounds().contains(mousePos)) {
				std::cout << "level no :" << this->LevelName.getString().toAnsiString()  << " button is clicked" << '\n';
				std::string LevelName = this->LevelName.getString().toAnsiString();
				std::cout << LevelName << '\n';
				if(LevelName == "TUTORIAL LEVEL") this->stateManager->pushState(std::make_unique<GameplayState>(this->stateManager , window)); 
				else std::cout << "printing other levels" << '\n';
			}

			this->isPressed = false;
		}
	}

	
}

void LevelButtons::updateCompletionStatusToComplete() {
	if(this->state == LevelButtonState::NotComplete) {
		this->state = LevelButtonState::Complete;

		this->LevelName.setFillColor(sf::Color::Green);
	}

	return;
}

void LevelButtons::updateCompletionStatusToInComplete() {
	if(this->state == LevelButtonState::Complete) {
		this->state = LevelButtonState::NotComplete;

		this->LevelName.setFillColor(sf::Color::Blue);
	}

	return;
}

void LevelButtons::render(sf::RenderWindow& window , LevelButtons& lb) {
	window.draw(lb.shape);
	window.draw(lb.LevelName);
}

LevelMenu::LevelMenu(GameStateManager* stateManager , sf::RenderWindow& window) {

	this->stateManager = stateManager;
	if(!this->font.loadFromFile("assets/font.otf")) {
		std::cerr << "failed to load font file in levelmenustate.cpp" << '\n';
	}

	this->currIndex = 0;

	this->GameTitle.setFont(this->font);
	this->GameTitle.setCharacterSize(64);
	this->GameTitle.setFillColor(sf::Color::White);
	this->GameTitle.setString("REWIND : A LOST LOVE");

	sf::FloatRect GameTitleRect = this->GameTitle.getLocalBounds();

	this->GameTitle.setOrigin(GameTitleRect.left + GameTitleRect.width / 2.0f , GameTitleRect.top + GameTitleRect.height / 2.0f);
	this->GameTitle.setPosition(window.getSize().x / 2.0f , window.getSize().y / 6.0f);

	const float buttonWidth = 200.f , buttonHeight = 100.f , buttonY = window.getSize().y / 2.0f , spacing = 20.0f ;
	

	this->navigatorButtons.emplace_back( 100.0f , buttonY , buttonWidth , buttonHeight , &this->font , "LEFT" , 
	                                sf::Color(100 , 100 , 100) , sf::Color(150 , 150 , 150 ) , sf::Color(70 , 70 , 70) , sf::Color::White , 
				 [this , &window]() {
					//std::cout << "left button is clicked" << '\n';	
				        if(this->currIndex -  1 >= 0) this->currIndex--; 
				        else this->currIndex = this->levelButtons.size() - 1;
				 }
	);

	this->navigatorButtons.emplace_back( window.getSize().x - 250.f , buttonY , buttonWidth , buttonHeight , &this->font , "RIGHT" , 
	                                sf::Color(100 , 100 , 100) , sf::Color(150 , 150 , 150 ) , sf::Color(70 , 70 , 70) , sf::Color::White , 
				 [this , &window]() {
				//	std::cout << "right button is clicked" << '\n';	
				     if(this->currIndex + 1 < this->levelButtons.size()) this->currIndex++;
				     else this->currIndex = 0;
				 }
	);	


	for(int i = 0 ; i < 6 ; i++) {

		std::string LevelName = "LEVEL" + std::to_string(i + 1);
		if(i == 0) LevelName = "TUTORIAL LEVEL";
		LevelButtons lb(stateManager , 650.0f , window.getSize().y / 3.0f , 500.0f  , 500.0f  , &this->font , LevelName );

		if(i == 0) lb.updateCompletionStatusToComplete();

		this->levelButtons.push_back(lb);
	}


	

}


void LevelMenu::update(float deltaTime) {

	sf::Vector2i mousePos = sf::Mouse::getPosition();


	for(auto& it : this->navigatorButtons) {
		it.update(sf::Vector2f(mousePos.x , mousePos.y));
	}
}


void LevelMenu::handleInput(sf::RenderWindow& window) {
	sf::Event e;
	while(window.pollEvent(e)) {
		if(e.type == sf::Event::Closed) window.close();

		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));


		for(auto& buttons : this->navigatorButtons) {
			buttons.handleEvent(e , mousePos);
		}

		this->levelButtons[this->currIndex].handleInput(e , mousePos , window);


	} 
	
}


void LevelMenu::render(sf::RenderWindow& window) {

	sf::Event e;
	window.clear();
	window.draw(this->GameTitle);
	for(auto& buttons : this->navigatorButtons) {
		buttons.render(window);
	}

	this->levelButtons[currIndex].render(window, this->levelButtons[currIndex]);
	
}


