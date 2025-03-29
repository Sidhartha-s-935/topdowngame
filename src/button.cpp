
#include "../includes/button.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

Button::Button(float x , float y , float width , float height , sf::Font* font , const  std::string& text , const sf::Color& color , const sf::Color& hoverColor , const sf::Color& activeColor , const sf::Color& textColor , std::function<void()> callback) {
	this->font = font;
	this->idleColor = idleColor;
	this->hoverColor = hoverColor;
	this->isPressed = false;
	this->textColor = textColor;
	this->callback = callback;
	this->activeColor = activeColor;
	this->isPressed = false;
	this->isHovered = false;


	this->shape.setPosition(sf::Vector2f(x , y));
	this->shape.setSize(sf::Vector2f(width , height));
	this->shape.setFillColor(idleColor);

	this->text.setFont(*font);
	this->text.setString(text);
	this->text.setFillColor(textColor);
	this->text.setCharacterSize(24);

	sf::FloatRect textRect = this->text.getLocalBounds();

	this->text.setOrigin(textRect.left + textRect.width / 2.0f , textRect.top + textRect.height / 2.0f);
	this->text.setPosition(x + width / 2.0f , y + height / 2.0f);

}

void Button::update(const sf::Vector2f& mousePos) {
	this->shape.setFillColor(this->idleColor);
	this->isHovered = false;

	if(this->shape.getGlobalBounds().contains(mousePos)) {
		this->isHovered = true;

		this->shape.setFillColor(this->hoverColor);

		if(this->isPressed) {
			this->shape.setFillColor(this->activeColor);
		}
	}
}

void Button::handleEvent(const sf::Event& event , const sf::Vector2f & mousePos) {
	if(event.type == sf::Event::MouseButtonPressed) {

		if(event.mouseButton.button == sf::Mouse::Left) {
			if(this->shape.getGlobalBounds().contains(mousePos)) this->isPressed = true;
		}

	}

	if(event.type == sf::Event::MouseButtonReleased) {
		if(event.mouseButton.button == sf::Mouse::Left) {
			if(this->isPressed && this->shape.getGlobalBounds().contains(mousePos)) {
				this->callback();

			}

			this->isPressed = false;
		}
		
	}
}


void Button::render(sf::RenderWindow& window) {
	window.draw(this->shape);
	window.draw(this->text);
}

