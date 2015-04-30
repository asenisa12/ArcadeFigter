#pragma once

#include "GameObject.h"

class GameLabel : public GameObject
{
protected:
	int width_, height_;
	SDL_Rect clip;
	std::string path_;

public:
	GameLabel(Value& data, int scrW, int scrH);
	bool loadMedia(SDL_Renderer* gRenderer);
	void changeWidth(int w);
	void renderLabel(int posX, int posY, SDL_Renderer* gRenderer);
	int getW();
	int getH();
};