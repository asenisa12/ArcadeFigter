#include "GameCharacter.h"

GameCharacter::GameCharacter(SquareGrid *grid, int characterType_,
	int screenW, int screenH)
	:levelGrid(grid), characterType(characterType_), currentCondition(STANDING),
	frame(0), screenH_(screenH), screenW_(screenW), flipType(SDL_FLIP_NONE), framesToEnd(DEATH_FRAMES)
{}

void GameCharacter::renderCharacter(SDL_Renderer* gRenderer)
{
	//Render current frame
	render(posX_, posY_, currentClip, 0, NULL, flipType, gRenderer, mWidth, mHigth);
}

Mix_Chunk* GameCharacter::loadWAV(std::string name)
{
	Mix_Chunk *chunk = Mix_LoadWAV(name.c_str());
	if (chunk == NULL)
		printf("Failed to load sound! SDL_mixer Error: %s\n", Mix_GetError());
	return chunk;
}

int GameCharacter::playSound(Mix_Chunk* chunk)
{
	return Mix_PlayChannel(-1, chunk, 0);
}
int GameCharacter::squareSize()
{
	return screenW_/GRID_SIZE_X;
}

int GameCharacter::getBottomY()
{
	return mHigth + posY_;
}

Location* GameCharacter::getCurrSquare()
{
	return currentSquare;
}

int  GameCharacter::getLocationCol(Location location)
{
	return location.X / squareSize();
}
int  GameCharacter::getLocationRow(Location location)
{
	return (location.Y - levelGrid->getStartingY()) / squareSize();
}

void GameCharacter::posToSquareMiddle()
{
	posX_ = currentSquare[FIRST_SQUARE_ID].X + squareSize() / 2;
	posY_ = currentSquare[FIRST_SQUARE_ID].Y - squareSize() / 2 - mHigth;
}

void GameCharacter::changeSquare(int row, int col)
{
	for (int i = 0; i < CHARACTER_SQUARES_NUM; i++)
	{
		levelGrid->change_cost(currentSquare[i], FREE_COST);
		currentSquare[i] = levelGrid->getLocation(row, col + i);
		levelGrid->change_cost(currentSquare[i], OCCUPATION_COST);
	}
}

void GameCharacter::changeCurrSquare(int dir)
{
	int adjX = 0, adjY = 0;

	if (Row_ < GRID_SIZE_Y && Row_ >= 0 && (dir == UP || dir == DOWN))
	{
		switch (dir)
		{
		case UP:
			Row_--;
			break;
		case DOWN:
			Row_++;
			break;
		}
	}
	
	if (Col_ < GRID_SIZE_X && Col_ >= 0 && (dir==LEFT || dir==RIGHT))
	{
		switch (dir)
		{
		case LEFT:
			Col_--;
			break;
		case RIGHT:
			Col_++;
			break;
		}
	}
	changeSquare(Row_, Col_);
	posX_ = currentSquare[FIRST_SQUARE_ID].X + squareSize() / 2;
}

void GameCharacter::manageSquareShift()
{
	if (shifting.X>=squareSize())
	{
		changeCurrSquare(RIGHT);
		shifting.X = 0;
	}
	else if (shifting.X <= squareSize()*(-1))
	{
		changeCurrSquare(LEFT);
		shifting.X = 0;
	}
	if (shifting.Y>=squareSize())
	{
		changeCurrSquare(DOWN);
		shifting.Y = 0;
	}
	else if (shifting.Y <= squareSize() * (-1))
	{
		changeCurrSquare(UP);
		shifting.Y = 0;
	}
}

void GameCharacter::setGridAttributes(Location location)
{
	int i = 0;
	currentSquare[i] = location;
	location.X += squareSize();
	location.Y += squareSize();
	currentSquare[i++] = location;
	shifting = { 0, 0 };
}

void GameCharacter::moveRight()
{
	//max x = 92% from screen w
	if (posX_ < (screenW_*0.96) && moveDir.right)
	{
		posX_ += movSpeed;
		shifting.X += movSpeed;
		currentCondition = MOVING;
	}
	flipType = SDL_FLIP_NONE;
}

void GameCharacter::moveLeft()
{
	if (posX_ > 10 && moveDir.left)
	{
		posX_ -= movSpeed;
		shifting.X -= movSpeed;
		currentCondition = MOVING;
	}
		flipType = SDL_FLIP_HORIZONTAL;

}

void GameCharacter::moveUp()
{
	currentCondition = MOVING;
	//max y = 72% form screen h
	if (getBottomY() > (screenH_*(0.72)) && moveDir.up){
		posY_ -= 2;
		shifting.Y -= 2;
		add--;
		movSpeed -= 0.05;
	}

}

void GameCharacter::moveDown()
{
	currentCondition = MOVING;
	//min y = 95% form screen h
	if (getBottomY() < (screenH_*(0.99)) && moveDir.down)
	{
		posY_ += 2;
		shifting.Y += 2;
		add++;
		movSpeed += 0.05;
	}

}

void GameCharacter::animation(std::string name)
{
	if (animationName!=name){
		firstclip = 0;
		Clips = animations[name];
		animationName = name;
	}
	else if (frame/4 > Clips.size())
	{
		frame = firstclip;
	}
}

void GameCharacter::loadAnimation(std::string animNames[])
{
	int y = 0;
	for (int i = 0; i < animFrameSize.size(); i++)
	{
		loadClips(animNames[i], y, animFrameSize[i]);
		y += CLIP_H;
	}
	animation("WALLKING");
	currentClip = &Clips[0];
	frame = 0;

	resizeClips(&Clips[frame / 4]);
	posToSquareMiddle();
}

void GameCharacter::loadClips(std::string name, int y, int endFrame)
{
	std::vector < SDL_Rect >  clips;
	int x = 0;
	for (int i = 0; i < endFrame; i++)
	{
		clips.push_back({ x, y, CLIP_W, CLIP_H });
		x += CLIP_W;
	}
	animations[name] = clips;
}

void GameCharacter::resizeClips(SDL_Rect *clip)
{
	currentClip = clip;
	//the height or width of the texture = clip w or h * 0.3% or 0.32% 
	//+ additional x or y pos if the charracter gets closer or away
	mWidth = (currentClip->w * (0.0030) *screenW_) + add;
	mHigth = (currentClip->h * (0.0032) *screenH_) + add;
}

bool GameCharacter::characterInLeft(GameCharacter* character)
{
	int colChar = character->getCol();
	if (abs(colChar - Col_) < 5 && colChar < Col_)
	{
		return true;
	}
	return false;
}
bool GameCharacter::characterInRigh(GameCharacter* character)
{
	int colChar = character->getCol();
	if (abs(colChar - Col_) < 5 && colChar > Col_)
	{
		return true;
	}
	return false;
}

void GameCharacter::collision(std::list<GameCharacter*> characters)
{
	moveDir = { true, true, true, true };
	for (std::list<GameCharacter*>::iterator it = characters.begin(); it != characters.end(); ++it)
	{

		GameCharacter* otherCharacter = *it;
		otherBottom = otherCharacter->getY() + otherCharacter->getHigth();
		otherLeft = otherCharacter->getX();
		otherRight = otherCharacter->getX() + otherCharacter->getWidth() / 2;

		myBottom = posY_ + mHigth;
		myLeft = posX_ + mWidth / 5;
		myRight = posX_ + mWidth / 2;

		if (abs(otherBottom - myBottom) < screenW_*0.03)
		{
			if (characterInLeft(otherCharacter))
			{
				moveDir.left = false;
			}
			if (characterInRigh(otherCharacter))
			{
				moveDir.right = false;
			}

			if (myLeft>otherLeft && myLeft < otherRight)
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

int GameCharacter::getCol()
{
	return Col_;
}
int GameCharacter::getRow()
{
	return Row_;
}

int GameCharacter::getCondition()
{
	return currentCondition;
}

void GameCharacter::editHealth(int damage)
{
	health -= damage;
	currentCondition = PUNCHED;
	if (health < 0) health = 0;
}

int GameCharacter::getHealth()
{
	return health;
}

SDL_RendererFlip GameCharacter::getFlipType()
{
	return flipType;
}

bool GameCharacter::ready_for_delete()
{
	if (framesToEnd <= 0) return true;
	return false;
}

int GameCharacter::CharacterType()
{
	return characterType;
}