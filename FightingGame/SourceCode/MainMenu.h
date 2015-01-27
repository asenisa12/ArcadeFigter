#pragma once

#include <string>
#include "GameState.h"
#include "BackGround.h"
#include "GameButton.h"
#include "GameLevel.h"

class MainMenu : public GameState
{
	BackGround *backGroundMenu;
	GameButton *startButton;
	GameButton *exitButton;
	bool loadObjects();
public:
	~MainMenu();
	virtual void update(GameStateMachine *stateMachine);
	virtual void render(SDL_Renderer* renderer);
	virtual bool onEnter(GameBase *mainGame_);
	virtual bool onExit();
	virtual std::string getStateID() const { return "MainMenu"; }
};