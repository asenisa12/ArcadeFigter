#ifndef GAME_BASE
#define GAME_BASE

#include <SDL_image.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "JsonReader.h"
#include "document.h"

class GameBase
{
	const int windowHeight, windowWidth;
	SDL_Window* gWindow_;
	SDL_Renderer* gRenderer_;
public:
	GameBase(int height, int width);
	~GameBase();
	bool init();
	int getScreenW();
	int getScreenH();
	SDL_Renderer* getRenderer();
	SDL_Window* getWindow();
	SDL_Event gameEvent;
	bool quit;
};

#endif