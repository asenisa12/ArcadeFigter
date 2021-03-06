#include "GameBase.h"

GameBase::GameBase(int height, int width)
	:windowHeight(height), windowWidth(width)
{}

GameBase::~GameBase()
{
	SDL_DestroyWindow(gWindow_);
	SDL_DestroyRenderer(gRenderer_);
	gWindow_ = NULL;
	gRenderer_ = NULL;

	SDL_Quit();
	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
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
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)<0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
		//sets texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	rapidjson::Document data = parse_jsonFile("Resources/settings.json");
	int windowFlag;
	if (data["fullScreen"].GetBool()) windowFlag = SDL_WINDOW_FULLSCREEN;
	else windowFlag = SDL_WINDOW_SHOWN;

	gWindow_ = SDL_CreateWindow("Arcade Fighter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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

	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}
	SDL_SetWindowFullscreen(gWindow_, windowFlag);
	return true;
}


SDL_Renderer* GameBase::getRenderer(){
	return gRenderer_;
}
SDL_Window* GameBase::getWindow(){
	return gWindow_;
}
