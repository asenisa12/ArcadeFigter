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

	//max x = 92% from screen w
	if (posX_ < (screenW_*(0.92)) && moveDir.right)
		posX_ += movSpeed;
	flipType = SDL_FLIP_NONE;
}

void GameCharacter::moveLeft()
{

	if (posX_ > 3 && moveDir.left)
		posX_ -= movSpeed;
	flipType = SDL_FLIP_HORIZONTAL;

}

void GameCharacter::moveUp()
{
	//max y = 42% form screen h
	if (posY_ > (screenH_*(0.42)) && moveDir.up){
		posY_ -= 2;
		add--;
		movSpeed -= 0.05;
	}

}

void GameCharacter::moveDown()
{
	//min y = 54% form screen h
	if (posY_ < (screenH_*(0.56)) && moveDir.down)
	{
		posY_ += 2;
		add++;
		movSpeed += 0.05;
	}

}

void GameCharacter::resizeClips(SDL_Rect Clips[])
{
	currentClip = &Clips[frame / 4];
	//the height or width of the texture = clip w or h * 0.3% or 0.32% 
	//+ additional x or y pos if the charracter gets closer or away
	mWidth = (currentClip->w * (0.0030) *screenW_) + add;
	mHigth = (currentClip->h * (0.0032) *screenH_) + add;
}

void GameCharacter::collision(GameCharacter* enemy[], int charactersCount)
{
	moveDir = { true, true, true, true };
	for (int i = 0; i < charactersCount; i++)
	{

		int enemyBottom = enemy[i]->getY() + enemy[i]->getHigth();
		int enemyLeft = enemy[i]->getX();
		int enemyRight = enemy[i]->getX() + enemy[i]->getWidth() / 2;

		int playerBottom = posY_ + mHigth;
		int playerLeft = posX_ + mWidth / 5;
		int playerRight = posX_ + mWidth / 2;

		if (abs(enemyBottom - playerBottom) < 20)
		{
			if (abs(enemyRight - playerLeft)<5 && enemyRight<playerLeft)
			{
				moveDir.left = false;
			}
			if (abs(enemyLeft - playerRight)<5 && enemyLeft > playerRight)
			{
				moveDir.right = false;
			}
			if (abs(playerLeft - enemyLeft) < 50)
			{
				if (enemyBottom < playerBottom)
				{
					moveDir.up = false;
				}
				if (enemyBottom > playerBottom)
				{
					moveDir.down = false;
				}

			}
		}
	}
}
