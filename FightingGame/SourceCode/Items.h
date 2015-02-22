#pragma once

#include "GameLabel.h"
#include "Grid.h"
#include <stdlib.h>
#include <time.h>
using namespace grid;
static int scrW=640, scrH= 480;
static const int ITEM_TYPES = 3;

class Item : public GameLabel
{
	int health_;
public:
	bool grabed;
	Item(jsonObj data, int pX, int pY, int health)
		:GameLabel(data, scrW, scrH),
		health_(health), grabed(false)
	{
		posX_ = pX; 
		posY_ = pY;
	}
	void render(SDL_Renderer *gRenderer)
	{
		if (!grabed) renderLabel(posX_, posY_, gRenderer);
	}
	bool loadmedia(SDL_Renderer *gRenderer)
	{
		return loadMedia(gRenderer);
	}
	int getHealth()
	{
		return health_;
	}
};

class Items
{
	SquareGrid *grid_;
	enum{Data = 0, Health = 1};
	std::vector<Item*> items;
public:
	Items(jsonObj data, SquareGrid *grid, int itemsCount);
	Item* operator[](int i);
	bool loadMedia(SDL_Renderer *gRenderer);

};