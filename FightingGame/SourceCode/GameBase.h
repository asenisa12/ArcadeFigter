#ifndef GAME_BASE
#define GAME_BASE

#include <SDL_image.h>
#include <SDL.h>
#include <stdio.h>

class GameBase
{
	const int windowHeight, windowWidth;
	SDL_Window* gWindow_;
	SDL_Renderer* gRenderer_;
public:
	GameBase(int height, int width);
	~GameBase();
	bool init();
	SDL_Renderer* getRenderer();
	SDL_Window* getWindow();
};

#endif