#include "GameLabel.h"

GameLabel::GameLabel(Value& data, int scrW, int scrH)
{
	path_ = data["texture"].GetString();
	width_ = scrW / data["wDelimiter"].GetInt();
	height_ = scrH / data["hDelimiter"].GetInt();
	clip = { 0, 0, data["clipW"].GetInt(), data["clipH"].GetInt() };
}
bool GameLabel::loadMedia(SDL_Renderer* gRenderer)
{
	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	return true;
}

void GameLabel::changeWidth(int w)
{
	width_ = w;
}
void GameLabel::renderLabel(int posX, int posY, SDL_Renderer* gRenderer)
{
	render(posX, posY, &clip, 0, NULL, SDL_FLIP_NONE, gRenderer, width_, height_);
}
int GameLabel::getW()
{
	return width_;
}
int GameLabel::getH()
{
	return height_;
}