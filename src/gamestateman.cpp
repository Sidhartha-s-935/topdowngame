#include "../includes/gamestateman.hpp"
#include <iostream>

void GameStateManager::pushState(std::unique_ptr<GameState> state) {
  this->states.push(std::move(state));
}

void GameStateManager::popState() {
  if (!this->states.empty()) this->states.pop(); 
}

void GameStateManager::changeState(std::unique_ptr<GameState> state) {
	if(!this->states.empty()) this->states.pop();

	this->states.push(std::move(state));
}

GameState *GameStateManager::getCurrentState() {
	if(this->states.empty()) return nullptr;
	return this->states.top().get();
}

bool GameStateManager::isEmpty()  { return this->states.empty(); }


void GameStateManager::print() {
    std::stack<std::unique_ptr<GameState>> tempStack;
    
    std::stack<std::unique_ptr<GameState>> tempStates = std::move(this->states);

    while (!tempStates.empty()) {
        std::cout << tempStates.top().get() << '\n'; 
        tempStack.push(std::move(tempStates.top())); 
        tempStates.pop();
    }

    while (!tempStack.empty()) {
        this->states.push(std::move(tempStack.top()));
        tempStack.pop();
    }
}

