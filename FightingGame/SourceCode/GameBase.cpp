#include "GameBase.h"

GameBase::GameBase(int height, int width)
	:windowHeight(height), windowWidth(width)
{}

GameBase::~GameBase()
{
	Mix_Quit();
	SDL_DestroyWindow(gWindow_);
	SDL_DestroyRenderer(gRenderer_);
	gWindow_ = NULL;
	gRenderer_ = NULL;

	SDL_Quit();
	IMG_Quit();
}

int GameBase::getScreenW()
{
	return windowWidth;
}

int GameBase::getScreenH()
{
	return windowHeight;
}

bool GameBase::init()
{
	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
		//sets texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	gWindow_ = SDL_CreateWindow("Game DEMO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	if (gWindow_ == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	
	gRenderer_ = SDL_CreateRenderer(gWindow_, -1, 
		SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

	if (gRenderer_ == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer_, 0xff, 0xff, 0xff, 0xff);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags)&IMG_INIT_PNG ))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}


	return true;
}


SDL_Renderer* GameBase::getRenderer(){
	return gRenderer_;
}
SDL_Window* GameBase::getWindow(){
	return gWindow_;
}
