#pragma once

#include <string>
#include <fstream>
#include <map>
#include "GameState.h"
#include "BackGround.h"
#include "GameButton.h"
#include "GameLevel.h"

static const std::string menuData ="Resources/menuData.json";

class MainMenu : public GameState
{
	enum { Start, Exit, p2Mode, p1Mode, Level1, Level2 };
	enum{Menu1, Menu2, Menu3};
	bool gameMode, level;
	int currentMenu;
	BackGround *backGroundMenu;
	std::map<int, GameButton*> button;
	bool loadObjects();
public:
	~MainMenu();
	virtual void update(GameStateMachine *stateMachine);
	virtual void render(SDL_Renderer* renderer);
	virtual bool onEnter(GameBase *mainGame_);
	virtual bool onExit();
	virtual std::string getStateID() const { return "MainMenu"; }
};