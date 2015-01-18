#pragma once

#include <vector>
#include <string>
#include <SDL.h>

class GameStateMachine;

class GameState
{
public: 
	virtual void update(GameStateMachine *stateMachine) = 0;
	virtual void render(SDL_Renderer* renderer) = 0;

	virtual bool onEnter() = 0;
	virtual bool onExit() = 0;

	virtual std::string getStateID() const = 0;
};


class GameStateMachine
{
	std::vector<GameState*> states;
public:
	void changeState(GameState *newState);
	void pushState(GameState *newState);
	void popState();
	void update();
	void render(SDL_Renderer* renderer);
};