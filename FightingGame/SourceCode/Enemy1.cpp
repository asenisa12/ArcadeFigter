#include "Enemy1.h"

Enemy1::Enemy1(std::string path, int posX, int posY)
{
	path_ = path;
	posX_ = posX;
	posY_ = posY;
	flipType = SDL_FLIP_NONE;
}

bool Enemy1::loadMedia(SDL_Renderer* gRenderer)
{
	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	mWidth = 90;
	mHigth = 160;
	Clips[0] = {0,0,100,100};
	currentClip = Clips;

}


Enemy1::~Enemy1()
{
	free();
}