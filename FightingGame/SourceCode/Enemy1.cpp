#include "Enemy1.h"

Enemy1::Enemy1(std::string path, int posX, int posY, int screenW, int screenH)
{
	screenH_ = screenH;
	screenW_ = screenW;
	path_ = path;
	posX_ = posX;
	posY_ = posY;
	movSpeed = screenW_*0.004;
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
	int x = 0;
	int y = 0;
	for (int i = 0; i < PUNCHING_ANIMATION_END;i++)
	{
		if (i == WALKING_ANIMATION_END)
		{
			x = 0;
			y += CLIP_H;
		}
		Clips[i].x = x;
		Clips[i].y = y;
		Clips[i].w = CLIP_W;
		Clips[i].h = CLIP_H;
		x += CLIP_W;
	}

	currentClip = Clips;
	frame = 0;
}

Enemy1::~Enemy1()
{
	free();
}

void Enemy1::doActions(GameCharacter* character[])
{
	collision(character,3);
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
		frame++;
	}
	else if (abs(posX_ - character[2]->getX())>5)
	{
		if (posX_ > character[2]->getX())
		{
			moveLeft();
		}
		if (posX_ < character[2]->getX())
		{
			moveRight();
		}
		frame++;
	}
	if (frame/4 == WALKING_ANIMATION_END)
	{
		frame = 0;
	}
	resizeClips(Clips);
}