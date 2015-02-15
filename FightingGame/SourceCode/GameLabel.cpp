#include "GameLabel.h"

GameLabel::GameLabel(jsonObj data, int scrW, int scrH)
{
	path_ = utility::conversions::to_utf8string(
		data[U("texture")].as_string());
	width_ = scrW / data[U("wDelimiter")].as_integer();
	height_ = scrH / data[U("hDelimiter")].as_integer();
	clip = { 0, 0, data[U("clipW")].as_integer(), data[U("clipH")].as_integer() };
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