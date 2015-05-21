#include "Items.h"


Items::Items(Value& data, SquareGrid *grid, int itemsCount)
{
	Value& itemArr = data;
	srand(time(NULL));
	for (int i = 0; i < itemsCount; i++)
	{
		int itemType = rand() % ITEM_TYPES;
		Value& itemData = itemArr[itemType];
		Value& attr = itemData[Health];

		int row = rand() % (GRID_SIZE_Y - Y_LIMIT) + Y_LIMIT;
		int col = rand() % (GRID_SIZE_X - X_LIMIT) + X_LIMIT/2;
		Location location = grid->getLocation(row, col);
		int pX = location.X;
		int pY = location.Y;

		items.push_back(new Item(itemData[Data], pX, pY, attr["health"].GetInt()));
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