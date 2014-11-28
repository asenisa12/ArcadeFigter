#include "Enemy1.h"

Enemy1::Enemy1(std::string path, int posX, int posY, int screenW, int screenH)
{
	screenH_ = screenH;
	screenW_ = screenW;
	path_ = path;
	posX_ = posX;
	posY_ = posY;
	movSpeed = screenW_*0.00625;
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

void Enemy1::doActions(GameCharacter* character[])
{
	collision(character);
	int playerbottomY = character[2]->getBottomY();
	if (abs(playerbottomY -getBottomY())>5){
		if (playerbottomY > getBottomY())
		{
			moveDown();
		}
		else if(playerbottomY < getBottomY())
		{
			moveUp();
		}
	}
}