#pragma once

#include "GameLabel.h"
#include "Grid.h"
#include <stdlib.h>
#include <time.h>
using namespace grid;
static int scrW=640, scrH= 480;

class Item
{
	GameLabel *label;
	int health_;
	int posX_;
	int posY_;
public:
	bool grabed;
	Item(jsonObj data, int pX, int pY, int health)
		:label(new GameLabel(data, scrW, scrH)), posX_(pX), posY_(pY),
		health_(health), grabed(false)
	{}
	void render(SDL_Renderer *gRenderer)
	{
		if (!grabed) label->renderLabel(posX_, posY_, gRenderer);
	}
	bool loadMedia(SDL_Renderer *gRenderer)
	{
		if (!label->loadMedia(gRenderer)) return false;
		return true;
	}
	int getHealth()
	{
		return health_;
	}
	~Item()
	{
		delete label;
	}
};

class Items
{
	SquareGrid *grid_;

	std::vector<Item*> items;
	void createItems(jsonObj data, SquareGrid *grid);
public:
	Items(jsonObj data, SquareGrid *grid, int itemsCount);
	Item* operator[](int i);
	bool loadMedia(SDL_Renderer *gRenderer);

};