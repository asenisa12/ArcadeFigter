#pragma once

#include "GameObject.h"
#include <string>
#include <SDL.h>

class BackGround : public GameObject
{
	static const int MAP_W = 3065;
	int camera_pos;
	std::string path_;
public:
	BackGround(std::string path);
	void renderBack(SDL_Rect *clip, SDL_Renderer* gRenderer);
	bool loadMedia(SDL_Renderer* gRenderer);
	~BackGround();
};