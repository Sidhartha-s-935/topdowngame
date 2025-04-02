
#ifndef GAME_STATE_MANAGER
#define GAME_STATE_MANAGER

#include "gamestate.hpp"
#include <memory>
#include <stack>

class GameStateManager {
private:
	  std::stack<std::unique_ptr<GameState>> states;

public:
	  void pushState(std::unique_ptr<GameState> state);
	  void popState();
	  void changeState(std::unique_ptr<GameState> state);
	  GameState *getCurrentState();
	  bool isEmpty();
	  void print();
};


#endif
