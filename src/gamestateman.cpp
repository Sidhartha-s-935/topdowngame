#include "../includes/gamestateman.hpp"

void GameStateManager::pushState(std::unique_ptr<GameState> state) {
  states.push(std::move(state));
}

void GameStateManager::popState() {
  if (!states.empty()) {
    states.pop();
  }
}

void GameStateManager::changeState(std::unique_ptr<GameState> state) {
  if (!states.empty()) {
    states.pop();
  }
  states.push(std::move(state));
}

GameState *GameStateManager::getCurrentState() {
  if (states.empty()) {
    return nullptr;
  }
  return states.top().get();
}

bool GameStateManager::isEmpty() const { return states.empty(); }
