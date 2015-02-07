#pragma once

#include "Grid.h"
#include "BackGround.h"
#include "GameState.h"
#include "Enemy1.h"
#include "Player.h"
#include "GameCharacter.h" 
#include <list>


class GameLevel : public GameState
{
	static const pKeys player1Keys;
	static const pKeys player2Keys;

	bool gameMode1, level1;

	grid::SquareGrid *levelgrid;
	SDL_Rect camera;
	BackGround *backGroundLevel1;
	static const std::string levelID;
	std::list<Player*> players;
	std::list<GameCharacter*> charactersList;
	void drawPlayerHealthBar(int health);
	bool createLevel();
public:
	~GameLevel();
	GameLevel(bool gameMode_, bool level_);
	bool LoadObjects();
	virtual void update(GameStateMachine *stateMachine);
	virtual void render(SDL_Renderer* renderer);
	virtual bool onEnter(GameBase *mainGame_);
	virtual bool onExit();
	virtual std::string getStateID() const { return levelID; }
};