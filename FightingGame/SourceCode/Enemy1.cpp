#include "Enemy1.h"

Enemy1::Enemy1(std::string path, int posX, int posY)
{
	path_ = path;
	posX_ = posX;
	posY_ = posY;
}

bool Enemy1::loadMedia(SDL_Renderer* gRenderer)
{
	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	mWidth = 100;
	mHigth = 100;
	Clips[0] = {0,0,100,100};

}

void Enemy1::renderEnemy1(SDL_Renderer* renderer)
{
	render(posX_, posY_, Clips, 0, NULL, SDL_FLIP_NONE, renderer, 100, 100);
}

Enemy1::~Enemy1()
{
	free();
}