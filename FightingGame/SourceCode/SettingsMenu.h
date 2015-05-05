#pragma once 

#include "GameButton.h"
#include "GameBase.h"
#include "JsonReader.h"

#include <string>
#include <fstream>
#include <vector>
#include <SDL_ttf.h>



enum{FullScr=0, Windowed=1, Plus=2, Minus=3};

class SettingsMenu
{	
	static const int MAX_VOLUME = 128;
	int volume;
	std::vector<GameButton*> button;
	std::string filePath;
	Document dataSettings;
	GameBase* mainGame;
	class VolumeBar
	{
		SDL_Rect bar;
		TTF_Font* font;
		SDL_Surface* surfaceMessage;
		SDL_Texture* Message;
		SDL_Rect Message_rect;
		int max_w;
		int Width;
		int value;
	public:
		VolumeBar(int x, int y, int w, int h, int value_, SDL_Renderer *renderer)
		{
			font = TTF_OpenFont("Resources/Font.ttf", 28);
			if (font == NULL)
			{
				printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
			}
			surfaceMessage = TTF_RenderText_Solid(font, "VOLUME", { 255, 255, 0 });

			SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
			if( Message == NULL )
			{
				printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
			}
			Message_rect = {x+w/2-w/4, y, w/2, h};
			bar = { x, y, w, h };
			max_w = w;
		}
		void drawBar(SDL_Renderer * renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0);
			SDL_RenderFillRect(renderer, &bar);
			SDL_RenderDrawRect(renderer, &bar);
			SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
		}
		void reduceVal()
		{
			if (bar.w > 0) bar.w -= max_w / 8; 
			if (bar.w < 0) bar.w = 0; 
		}
		void incrVal()
		{ 
			if (bar.w < max_w) bar.w += max_w / 8;
			if (bar.w > max_w) bar.w = max_w;
		}
		int getValue(){ return value; }

		~VolumeBar()
		{
			SDL_DestroyTexture(Message);
			SDL_FreeSurface(surfaceMessage);
			TTF_CloseFont(font);
		}

	} *volumeBar;
public:
	bool loadMedia();
	void render();
	void update();
	SettingsMenu(std::string path, GameBase *mainGame_);
	~SettingsMenu();
};