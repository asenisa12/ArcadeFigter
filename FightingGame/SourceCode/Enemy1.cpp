#include "Enemy1.h"

Enemy1::Enemy1(std::string path, int posX, int posY, int screenW, int screenH, GameCharacter* player)
	:player_(player), action(false)
{
	characterType = GENEMY;
	currentCondition = STANDING;
	health = 100;
	punchStart_count = 0;
	screenH_ = screenH;
	screenW_ = screenW;
	path_ = path;
	posX_ = posX;
	posY_ = posY;
	//movement speed == 0.5% from screen width
	movSpeed = screenW_*0.005;
	flipType = SDL_FLIP_NONE;
}

const double Enemy1::SHIFTING_PERCENTIGE = 0.1;
const double Enemy1::DIFF_BY_X_PERCENTIGE = 0.03;
const double Enemy1::DIFF_BY_Y_PERCENTIGE = 0.04;

bool Enemy1::loadMedia(SDL_Renderer* gRenderer)
{
	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	mWidth =200;
	mHigth = 150;
	int x = 0;
	int y = 0;
	for (int i = 0; i < FALLING_ANIMATION_END;i++)
	{
		if (i == WALKING_ANIMATION_END || i== PUNCHING_ANIMATION_END)
		{
			x = 0;
			y += CLIP_H;
		}
		Clips[i].x = x;
		Clips[i].y = y;
		Clips[i].w = CLIP_W;
		Clips[i].h = CLIP_H;
		if (i == 15 || i == 16)
		{
			x += FALLING_CLIP_W;
		}
		else x += CLIP_W;
		
	}

	currentClip = Clips;
	frame = 0;
	return true;
}

Enemy1::~Enemy1()
{
	free();
}

void Enemy1::punch()
{
	currentCondition = PUNCHING;
	animation(PUNCHING_ANIMATION_END, WALKING_ANIMATION_END);
}

void Enemy1::fall()
{
	currentCondition = PUNCHED;
	animation(FALLING_ANIMATION_END, PUNCHING_ANIMATION_END);
}

bool Enemy1::moveToPosition(int X, int Y)
{
	diffrenceY = abs(Y - getBottomY());
	diffrenceX = abs(posX_ - X);
	if (diffrenceY > screenH_ * DIFF_BY_Y_PERCENTIGE)
	{
		if (Y > getBottomY() )
		{
			if (moveDir.down)
			{
				moveDown();
				return true;
			}

		}
		else if (Y < getBottomY() )
		{
			if (moveDir.up)
			{
				moveUp();
				return true;
			}
		}
	}
	else if (posX_ > X )
	{
		if (moveDir.left )
		{
			moveLeft();
			return true;
		}
	}
	else if (posX_ < X)
	{
		if (moveDir.right )
		{
			moveRight();
			return true;
		}
	}
	return false;
}

void Enemy1::moving()
{
	/*if (!moveDir.down && diffrenceX<DIFF_BY_X_PERCENTIGE * screenW_){
		printf("aaaaaa");
		if (posX_ > player_->getX()){
			posX_ += screenW_*SHIFTING_PERCENTIGE;
		}
		else
		{
			posX_ -= screenW_*SHIFTING_PERCENTIGE;
		}
		action = true;
	}
	else if (!moveDir.up && diffrenceX<DIFF_BY_X_PERCENTIGE * screenW_)
	{
		posX_ -= screenW_*SHIFTING_PERCENTIGE;
		action = true;
	}*/

	if (!action)
	{
		int playerbottomY = player_->getBottomY();
		diffrenceY = abs(player_->getBottomY() - getBottomY());
		diffrenceX = abs(posX_ - player_->getX());
		if (!moveDir.down && diffrenceX<DIFF_BY_X_PERCENTIGE * screenW_)
		{
			if (posX_ > player_->getX())
			{
				destX = player_->getX() + screenW_*SHIFTING_PERCENTIGE;
				destY = player_->getBottomY() + screenW_*SHIFTING_PERCENTIGE;
			}
			else
			{
				destX = player_->getX() - screenW_*SHIFTING_PERCENTIGE;
				destY = player_->getBottomY() - screenW_*SHIFTING_PERCENTIGE;
			}
		}
		else
		{
			printf("aa");
			destX = player_->getX();
			destY = player_->getBottomY();
		}
	}

	action  = moveToPosition(destX, destY);
}

void Enemy1::doActions(SDL_Rect* camera, std::list<GameCharacter*> characters)
{
	punched = false;
	collision(characters);
	punched = player_->punching();
	otherLeft = player_->getX();
	otherRight = player_->getX() + player_->getWidth() / 2;

	if (punched && (characterInLeft() || characterInRigh()) && player_->getCondition() != PUNCHED)
	{
		fall();
	}
	else
	{
		lastclip = WALKING_ANIMATION_END;
		firstclip = 0;
		
		moving();

		if ((characterInLeft() || characterInRigh()) &&!punched)
		{
			if (frame/4 == PUNCHING_ANIMATION_END-1)
				player_->editHealth(DAMAGE);
			punch();
		}
		else if (action)
		{
			frame++;
		}

	}
	if (frame/4 >= lastclip)
	{
		frame = firstclip;
	}
	resizeClips(Clips);
}