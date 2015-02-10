#pragma once

#include "Grid.h"
#include "BackGround.h"
#include "GameState.h"
#include "Enemy1.h"
#include "Player.h"
#include "GameCharacter.h" 
#include <list>

enum { Start, Exit, p2Mode, p1Mode, Level1, Level2 };


class GameLevel : public GameState
{
	static const pKeys player1Keys;
	static const pKeys player2Keys;
	static const int CAMERA_POSITIONS = 5;

	int gameMode, level;
	int cameraPosCount;

	grid::SquareGrid *levelgrid;
	SDL_Rect camera;
	BackGround *backGroundLevel1;
	static const std::string levelID;
	std::list<Player*> players_;
	std::list<GameCharacter*> charactersList;
	void drawPlayerHealthBar(int health);
	bool createLevel();
	void manage_camera();
public:
	~GameLevel();
	GameLevel(int gameMode_, int level_);
	bool LoadObjects();
	virtual void handleEvent();
	virtual void update(GameStateMachine *stateMachine);
	virtual void render(SDL_Renderer* renderer);
	virtual bool onEnter(GameBase *mainGame_);
	virtual bool onExit();
	virtual std::string getStateID() const { return levelID; }
};