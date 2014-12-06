#include "GameCharacter.h"

void GameCharacter::renderCharacter(SDL_Renderer* gRenderer)
{
	//Render current frame
	render(posX_, posY_, currentClip, 0, NULL, flipType, gRenderer, mWidth, mHigth);
}

int GameCharacter::getBottomY()
{
	return mHigth + posY_;
}

void GameCharacter::moveRight()
{
	currentCondition = MOVING;
	//max x = 92% from screen w
	if (posX_ < (screenW_*(0.92)) && moveDir.right)
	{
		posX_ += movSpeed;
	}
	flipType = SDL_FLIP_NONE;
}

void GameCharacter::moveLeft()
{
	currentCondition = MOVING;
	if (posX_ > 3 && moveDir.left)
	{
		posX_ -= movSpeed;
	}
		flipType = SDL_FLIP_HORIZONTAL;

}

void GameCharacter::moveUp()
{
	currentCondition = MOVING;
	//max y = 42% form screen h
	if (posY_ > (screenH_*(0.42)) && moveDir.up){
		posY_ -= 2;
		add--;
		movSpeed -= 0.05;
	}

}

void GameCharacter::moveDown()
{
	currentCondition = MOVING;
	//min y = 54% form screen h
	if (posY_ < (screenH_*(0.56)) && moveDir.down)
	{
		posY_ += 2;
		add++;
		movSpeed += 0.05;
	}

}

void GameCharacter::animation(int last, int first)
{
	lastclip = last;
	firstclip = first * 4;
	if (frame / 4 < first ||
		frame / 4 >= lastclip)
	{
		frame = firstclip;
	}
	frame++;

}

void GameCharacter::resizeClips(SDL_Rect Clips[])
{
	currentClip = &Clips[frame / 4];
	//the height or width of the texture = clip w or h * 0.3% or 0.32% 
	//+ additional x or y pos if the charracter gets closer or away
	mWidth = (currentClip->w * (0.0030) *screenW_) + add;
	mHigth = (currentClip->h * (0.0032) *screenH_) + add;
}

bool GameCharacter::characterInLeft()
{
	if (abs(otherRight - myLeft) < 5 && otherRight < myLeft)
	{
		return true;
	}
	return false;
}
bool GameCharacter::characterInRigh()
{
	if (abs(otherLeft - myRight) < 5 && otherLeft > myRight)
	{
		return true;
	}
	return false;
}

void GameCharacter::collision(std::vector<GameCharacter*> characters)
{
	moveDir = { true, true, true, true };
	for (std::vector<GameCharacter*>::iterator it = characters.begin(); it != characters.end(); ++it)
	{

		GameCharacter* otherCharacter = *it;
		otherBottom = otherCharacter->getY() + otherCharacter->getHigth();
		otherLeft = otherCharacter->getX();
		otherRight = otherCharacter->getX() + otherCharacter->getWidth() / 2;

		myBottom = posY_ + mHigth;
		myLeft = posX_ + mWidth / 5;
		myRight = posX_ + mWidth / 2;

		if (abs(otherBottom - myBottom) < 30)
		{
			if (characterInLeft())
			{
				moveDir.left = false;
			}
			if (characterInRigh())
			{
				moveDir.right = false;
			}

			if (abs(myLeft - otherLeft) < 30)
			{
				if (otherBottom < myBottom)
				{
					moveDir.up = false;
				}
				if (otherBottom > myBottom)
				{
					moveDir.down = false;
				}

			}
		}
	}
}
bool GameCharacter::punching()
{
	return currentCondition == PUNCHING;
}

int GameCharacter::getCondition()
{
	return currentCondition;
}

void GameCharacter::editHealth(int damage)
{
	health -= damage;

	if (health < 0) health = 0;
}

int GameCharacter::getHealth()
{
	return health;
}

int GameCharacter::CharacterType()
{
	return characterType;
}