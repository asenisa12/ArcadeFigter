#ifndef MAINGAMEHEADER
#define MAINGAMEHEADER

#include "GameState.h"

#include "MainMenu.h"
#include "Player.h"
#include <iostream>
#include <SDL.h>
#include "Player.h"
#include "GameBase.h"
#include "GameObject.h"
#include "BackGround.h"
#include "GameButton.h"
#include "GameCharacter.h"
#include "Enemy1.h"
#include <string>
#include <list>
#include <unordered_map>
#include <algorithm>
#include "Grid.h"
#include "PathFinding.h"
#include "PriorityQueue.h"

using namespace std;

enum GameState1
{
	MainMenu1,
	Level1,
	Level2
};

extern int gameState;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int ENEMIES_LEVEL1;

extern bool started;

extern GameCharacter* character[];
extern std::list<GameCharacter*> charactersList;

extern SDL_Rect camera;

extern GameBase mainGame;
extern grid::SquareGrid levelgrid;
extern BackGround backGroundMenu;
extern BackGround backGroundLevel1;

extern Enemy1 enemy1;
extern Enemy1 enemy2;

extern GameButton startButton;
extern GameButton exitButton;

struct greater_than
{
	inline bool operator() (GameCharacter* struct1, GameCharacter* struct2);
};


#endif