#include "Enemy1.h"

Enemy1::Enemy1(std::string path, Location startlocation, int screenW, int screenH,
	GameCharacter* player, SquareGrid *grid)
	:player_(player), action(false), GameCharacter(grid, GENEMY, screenW, screenH, MAX_HEALTH)
{

	punchStart_count = 0;
	path_ = path;
	//movement speed == 0.5% from screen width
	movSpeed = screenW_*0.005;	
	setGridAttributes(startlocation);
	PrevSquare = currentSquare[0];
}

const double Enemy1::SHIFTING_PERCENTIGE = 0.1;
const double Enemy1::DIFF_BY_X_PERCENTIGE = 0.0046876;
const double Enemy1::DIFF_BY_Y_PERCENTIGE = 0.00625;

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
		Clips[i] = { x, y, CLIP_W, CLIP_H };
		if (i == 15 || i == 16)
		{
			x += FALLING_CLIP_W;
		}
		else x += CLIP_W;
		
	}

	currentClip = Clips;
	frame = 0;
	resizeClips(Clips);
	posToSquareMiddle();
	destY = posY_;
	destX = posX_;

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

void Enemy1::findDestination()
{
	path = (std::async(&Enemy1::getPath, this)).get();

	for (;;){
		if (currentSquare[FIRST_SQUARE_ID] == path.back() && path.back() != currentGoal)
		{
			path.pop_back();
		}
		else break;
	}
	destX = path.back().X + squareSize() / 2;
	destY = path.back().Y - squareSize() / 2 - mHigth;
}

void Enemy1::moveToPosition(int X, int Y)
{
	diffrenceY = abs(Y+mHigth - getBottomY());
	diffrenceX = abs(posX_ - X);

	if (diffrenceY >screenH_ * DIFF_BY_Y_PERCENTIGE)
	{
		if (Y > posY_)
		{
			if (moveDir.down)
			{
				moveDown();
				action = true;
				return;
			}

		}
		else if (Y <posY_)
		{
			if (moveDir.up)
			{
				moveUp();
				action = true;
				return;
			}
		}
	}
	else if (diffrenceX >screenW_ * DIFF_BY_X_PERCENTIGE)
	{
		if (posX_ > X)
		{
			if (moveDir.left)
			{
				moveLeft();
				action = true;
				return;
			}
		}
		else if (posX_ < X)
		{
			if (moveDir.right)
			{
				moveRight();
				action = true;
				return;
			}
		}
	}
	if (diffrenceY < screenH_ * DIFF_BY_Y_PERCENTIGE)
	{
		posX_ =X;
		action = true;
	}
	if (diffrenceX < screenW_ * DIFF_BY_X_PERCENTIGE)
	{
		posY_ = Y;
		action = true;
	}
}

void Enemy1::moving()
{
	if (currentGoal != currentSquare[FIRST_SQUARE_ID])
	{

		if (posX_ != destX || posY_ != destY)
		{
			moveToPosition(destX, destY);

			if (posX_ == destX && posY_ == destY)
			{
				changeSquare(getRow(path.back()), getCol(path.back()));
			}
		}
		else
		{	
				findDestination();
				printf("DESTINATION posX: %d posY: %d\n", destX, destY);
		}
	}
}

Location Enemy1::getPlayerSquare()
{
	Location playerLocation = player_->getCurrSquare()[FIRST_SQUARE_ID];
	int playerRow = getRow(playerLocation);
	int playerCol = getCol(playerLocation);
	int adj = 0;

	if (playerCol>3)
	{
		playerCol -= 5;
	}
	else
	{
		playerCol += 1;
	}

	return levelGrid->getLocation(playerRow, playerCol + adj);
}

std::vector<Location> Enemy1::getPath()
{
	std::unordered_map<Location, Location, LocationHash, Equal> came_from;
	std::unordered_map<Location, int, LocationHash, Equal> cost_so_far;
	path_search(*levelGrid, currentSquare[FIRST_SQUARE_ID], currentGoal, came_from, cost_so_far);
	auto path = reconstruct_path(currentSquare[FIRST_SQUARE_ID], currentGoal, came_from);
	return path;
}

void Enemy1::doActions(SDL_Rect* camera, std::list<GameCharacter*> characters)
{
	punched = false;
	collision(characters);
	punched = player_->punching();
	otherLeft = player_->getX();
	otherRight = player_->getX() + player_->getWidth() / 2;
	currentCondition = STANDING;
	action = false;
	currentGoal = getPlayerSquare();

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