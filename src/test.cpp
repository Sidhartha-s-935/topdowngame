/*#include "../includes/player.hpp"*/
/*#include <SFML/Graphics/Rect.hpp>*/
/*#include <SFML/System/Vector2.hpp>*/
/*#include <SFML/Window/Keyboard.hpp>*/
/*#include <iostream>*/
/*#include <cmath>*/
/**/
/**/
/*Player::Player(float x , float y , float size , float speed) : speed(speed) {*/
/*	this->shape.setSize(sf::Vector2f(size , size));*/
/*	this->shape.setFillColor(sf::Color::Green);*/
/*	this->shape.setPosition(x , y);*/
/*	this->shape.setOrigin(size / 2.0f , size / 2.0f);*/
/*}*/
/**/
/*void Player::handleInput(const Map& map) {*/
/*	sf::Vector2f orgPos = this->shape.getPosition();*/
/*	sf::Vector2f movement(0.f , 0.f);*/
/**/
/*	bool movingDiagonally = false;*/
/**/
/*	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= this->speed;*/
/*	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += this->speed;*/
/*	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= this->speed;*/
/*	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += this->speed;*/
/**/
/*	if(movement.x != 0 && movement.y != 0) {*/
/*		movingDiagonally = true;*/
/*		float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);*/
/*		movement.x = movement.x / length * speed;*/
/*		movement.y = movement.y / length * speed;*/
/*	}*/
/**/
/*	if(movement.x != 0) {*/
/*		this->shape.move(movement.x , 0);*/
/*		if(map.isColliding(this->shape.getGlobalBounds())) {*/
/*			this->shape.setPosition(orgPos.x , this->getPosition().y);*/
/*		}*/
/*	}*/
/**/
/*	if(movement.y !=0 ) {*/
/*		this->shape.move(0 , movement.y);*/
/*		if(map.isColliding(this->shape.getGlobalBounds())) {*/
/*			this->shape.setPosition(this->shape.getPosition().x , orgPos.y);*/
/*		}*/
/*	}*/
/*}*/
/**/
/*void Player::update(const Map& map) {*/
/*	handleInput(map);*/
/*}*/
/**/
/*void Player::draw(sf::RenderWindow& window) {*/
/*	window.draw(this->shape);*/
/*}*/
/**/
/**/
/*sf::FloatRect Player::getBounds() const {*/
/*	return this->shape.getGlobalBounds();*/
/*}*/
/**/
/*sf::Vector2f Player::getPosition() const {*/
/*	return this->shape.getPosition();*/
/*}*/
/**/
/*void Player::setScale(float scale) {*/
/*	float currentSize = this->shape.getSize().x;*/
/*}*/
