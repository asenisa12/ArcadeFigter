#include "Enemy1.h"

Enemy1::Enemy1(std::string path, int posX, int posY, int screenW, int screenH)
{
	currentCondition = STANDING;
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

void Enemy1::moving(GameCharacter* player, bool &action)
{
	int playerbottomY = player->getBottomY();
	int diffrenceY = abs(playerbottomY - getBottomY());
	int diffrenceX = abs(posX_ - player->getX());
	/*bool moveConditionL = ((diffrenceY<15 && playerbottomY) > getBottomY()) && diffrenceX<10;
	bool moveConditionR = ((diffrenceY<15 && playerbottomY) < getBottomY()) && diffrenceX<10;*/
	if (diffrenceY > 5){
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

	}else 
	if (posX_ > player->getX() )
	{
		moveLeft();
		if (moveDir.left)
			action = true;
	}else
		if (posX_ < player->getX())
	{
		moveRight();
		if (moveDir.right)
			action = true;
	}
}

void Enemy1::doActions(GameCharacter* character[])
{
	punched = false;
	collision(character,3);
	punched = character[2]->punching();
	enemyLeft = character[2]->getX();
	enemyRight = character[2]->getX() + character[2]->getWidth() / 2;

	bool action = false;

	if (punched && (characterInLeft() || characterInRigh()) && character[2]->getCondition()!=PUNCHED)
	{
		fall();
	}
	else
	{
		lastclip = WALKING_ANIMATION_END;
		firstclip = 0;
		
		moving(character[2], action);

		if ((characterInLeft() || characterInRigh()) &&!punched)
		{
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