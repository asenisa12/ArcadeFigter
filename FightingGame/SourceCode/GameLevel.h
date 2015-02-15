#pragma once

#include "Grid.h"
#include "BackGround.h"
#include "GameState.h"
#include "Enemy1.h"
#include "Player.h"
#include "GameCharacter.h" 
#include <list>

enum { Start = 0, Exit = 1, p1Mode = 2, p2Mode = 3, Level1 = 4, Level2 = 5 };


class GameLevel : public GameState
{
	static const pKeys player1Keys;
	static const pKeys player2Keys;
	static const int CAMERA_POSITIONS = 5;
	static const std::string levelJSON;
	std::string playerJSON;
	std::string enemyJSON;

	std::string textureBgraund;
	int gameMode, level;
	int cameraPosCount;

	std::vector<std::list<GameCharacter*>> enemies;
	grid::SquareGrid *levelgrid;
	SDL_Rect camera;
	BackGround *backGroundLevel1;
	static const std::string levelID;
	std::list<Player*> players_;
	std::list<GameCharacter*> charactersList;
	void drawPlayerHealthBar(int health);
	GameCharacter* get_enemy(jsonObj eneym);
	bool createLevel();
	void createPlayer(int id);
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