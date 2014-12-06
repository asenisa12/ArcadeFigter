#include "Enemy1.h"

Enemy1::Enemy1(std::string path, int posX, int posY, int screenW, int screenH, GameCharacter* player)
	:player_(player)
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
const double Enemy1::DIFF_BY_X_PERCENTIGE = 0.01;
const double Enemy1::DIFF_BY_Y_PERCENTIGE = 0.023;

bool Enemy1::loadMedia(SDL_Renderer* gRenderer)
{
	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
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

void Enemy1::moving(bool &action)
{
	int playerbottomY = player_->getBottomY();
	int diffrenceY = abs(playerbottomY - getBottomY());
	int diffrenceX = abs(posX_ - player_->getX());

	if (!moveDir.down && diffrenceX<DIFF_BY_X_PERCENTIGE * screenW_){
		if (posX_ > player_->getX()){
			posX_ += screenW_*SHIFTING_PERCENTIGE;
		}
		else
		{
			posX_ -= screenW_*SHIFTING_PERCENTIGE;
		}
		action = true;
	}else
	if (!moveDir.up && diffrenceX<DIFF_BY_X_PERCENTIGE * screenW_){
		posX_ -= screenW_*SHIFTING_PERCENTIGE;
		action = true;
	}
	if (diffrenceY > screenH_ * DIFF_BY_X_PERCENTIGE){
		if (playerbottomY > getBottomY())
		{
			moveDown();
			action = true;
		}
		else if (playerbottomY < getBottomY())
		{
			moveUp();
			action = true;
		}

	}
	else if (posX_ > player_->getX() )
	{
		moveLeft();
		if (moveDir.left ){
			action = true;
		}
	}
	else if (posX_ < player_->getX())
	{
		moveRight();
		if (moveDir.right ){
			action = true;
		}
	}
}

void Enemy1::doActions(SDL_Rect* camera, std::vector<GameCharacter*> characters)
{
	punched = false;
	collision(characters);
	punched = player_->punching();
	otherLeft = player_->getX();
	otherRight = player_->getX() + player_->getWidth() / 2;

	bool action = false;

	if (punched && (characterInLeft() || characterInRigh()) && player_->getCondition() != PUNCHED)
	{
		fall();
	}
	else
	{
		lastclip = WALKING_ANIMATION_END;
		firstclip = 0;
		
		moving(action);

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