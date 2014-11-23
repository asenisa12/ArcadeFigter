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
#include <string>


extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern bool started;
extern bool quit;

extern SDL_Rect camera;
extern SDL_Event gameEvent;

extern GameBase mainGame;

extern BackGround backGroundMenu;
extern BackGround backGroundLevel1;

extern Player player1;

extern GameButton startButton;
extern GameButton exitButton;

void update();
void render();
void getInput();

bool loadMedia();

#endif