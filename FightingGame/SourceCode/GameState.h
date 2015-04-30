#pragma once

#include "GameBase.h"
#include <vector>
#include <string>
#include <SDL.h>

class GameStateMachine;


class GameState
{
protected:
	GameBase *mainGame;
public: 
	virtual void update(GameStateMachine *stateMachine) = 0;
	virtual void render(SDL_Renderer* renderer) = 0;
	virtual void handleEvent() = 0;

	virtual bool onEnter(GameBase *mainGame_) = 0;
	virtual bool onExit() = 0;

	virtual std::string getStateID() const = 0;
};


class GameStateMachine
{
	std::vector<GameState*> states;
	GameBase *mainGame;
public:
	GameStateMachine(GameBase *mainGame_);
	GameBase* getGameBase();
	void changeState(GameState *newState);
	void pushState(GameState *newState);
	void popState();
	void handleEvent();
	void update();
	void render(SDL_Renderer* renderer);
};