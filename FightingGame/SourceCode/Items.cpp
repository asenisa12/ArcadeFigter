#include "Items.h"


Items::Items(jsonObj data, SquareGrid *grid, int itemsCount)
{
	web::json::array itemArr = data.as_array();
	srand(time(NULL));
	for (int i = 0; i < itemsCount; i++)
	{
		int itemType = rand() % ITEM_TYPES;
		web::json::array itemData = itemArr.at(itemType).as_array();
		jsonObj attr = itemData.at(Health);

		int row = rand() % GRID_SIZE_Y - 1 + 1;
		int col = rand() % GRID_SIZE_X - 10 + 10;
		Location location = grid->getLocation(row, col);
		int pX = location.X;
		int pY = location.Y;

		items.push_back(new Item(itemData.at(Data), pX, pY, attr[U("health")].as_integer()));
	}
}
Item* Items::operator[](int i)
{
	if (i<items.size() && i>-1)
	{
		return items[i];
	}
	else return NULL;
}

bool Items::loadMedia(SDL_Renderer *gRenderer)
{
	for (auto item : items)
		if (!item->loadmedia(gRenderer)) return false;
	return true;
}