#ifndef MAINGAMEHEADER
#define MAINGAMEHEADER

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
#include <vector>
#include <algorithm>



extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int ENEMIES_LEVEL1;

extern bool started;
extern bool quit;

extern GameCharacter* enemy[];
extern std::vector<GameCharacter*> characters;

extern SDL_Rect camera;
extern SDL_Event gameEvent;

extern GameBase mainGame;

extern BackGround backGroundMenu;
extern BackGround backGroundLevel1;

extern Player player1;
extern Enemy1 enemy1;
extern Enemy1 enemy2;

extern GameButton startButton;
extern GameButton exitButton;

void update();
void render();
void getInput();

bool loadMedia();

struct greater_than
{
	inline bool operator() (GameCharacter* struct1, GameCharacter* struct2);
};
#endif