#pragma once

#include "Grid.h"
#include "BackGround.h"
#include "GameState.h"
#include "Enemy1.h"
#include "Player.h"
#include "GameCharacter.h" 
#include "GameLabel.h"
#include "Items.h"
#include <list>
#include "HealthBar.h"

enum { Start = 0, Exit = 1, p1Mode = 2, p2Mode = 3, Level1 = 4, Level2 = 5, settings };

class GameLevel : public GameState
{
	enum{
		INGAME,
		GAME_OVER,
		WIN,
		ESKAPE_PRESS
	};

	static const pKeys player1Keys;
	static const pKeys player2Keys;
	static const int CAMERA_POSITIONS = 4;
	static const std::string levelJSON;
	int livingEnemies;
	bool quitLevel;
	std::string playerJSON;
	std::string enemyJSON;

	std::string levelTheme;
	std::string textureBgraund;
	int gameMode, level;
	int cameraPosCount;
	int currentState;
	int playersCount;
	Items *items;

	Mix_Music *gMusic = NULL;
	std::vector<std::list<GameCharacter*>> enemies;
	grid::SquareGrid *levelgrid;
	std::vector<HealthBar*> healthBars;
	SDL_Rect camera;
	BackGround *backGroundLevel1;
	GameLabel *gameOver;
	GameLabel *youWin;
	static const std::string levelID;
	std::list<Player*> players_;
	std::list<GameCharacter*> charactersList;
	void drawPlayerHealthBar();
	GameCharacter* get_enemy(Value& eneym);
	bool createLevel();
	void drawLabel(GameLabel *label);
	void createPlayer(int id);
	void manage_camera();
	void ingame();
	void enemy_add_target();
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