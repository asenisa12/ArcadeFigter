#include "Enemy1.h"

Enemy1::Enemy1(std::string path, Location startlocation, int screenW, int screenH,
	GameCharacter* player, SquareGrid *grid)
	:player_(player), action(false), GameCharacter(grid, GENEMY, screenW, screenH)
{

	health = 100;
	punchStart_count = 0;
	path_ = path;
	//movement speed == 0.5% from screen width
	movSpeed = screenW_*0.005;
	posX_ = startlocation.X + squareSize() / 2;
	posY_ = startlocation.Y - squareSize() / 2 + mHigth;

	destY = getBottomY();
	destX = posX_;
	setGridAttributes(startlocation);
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

void Enemy1::moveToPosition(int X, int Y)
{
	if (X>posX_)
	{
		printf("1\n");
		moveRight();
	}
	else if (X<posX_)
	{
		printf("2\n");
		moveLeft();
	}
	else if (Y>getBottomY())
	{
		printf("3\n");
		moveDown();
	}
	else if (Y<getBottomY())
	{
		printf("4\n");
		moveUp();
	}


}

void Enemy1::moving()
{

	if (currentGoal != getCurrSquare()[0])
	{
		if (posX_!=destX || getBottomY()!=destY)
		{
			/*printf("destxX:%d destY:%d\n",destX, destY);
			printf("posX:%d posY:%d\n", posX_, getBottomY());*/
			moveToPosition(destX, destY);
		}
		else
		{
			posX_ = destX;
			posY_ = destY - mHigth;
			auto path = (std::async(&Enemy1::getPath, this)).get();

			for (auto it : path)
			{
				printf("}{posX: %d posY: %d\n", it.X, it.Y);
			}

			destX = path.back().X + squareSize() / 2;
			destY = path.back().Y - squareSize() / 2;
			printf("DESTINATION posX: %d posY: %d\n", destX, destY);
		}
	}
	
}

Location Enemy1::getPlayerSquare()
{
	Location playerLocation = player_->getCurrSquare()[0];
	int playerRow = getRow(playerLocation);
	int playerCol = getCol(playerLocation);
	int adj = 0;

	if (playerCol>1)
	{
		adj -= 2;
	}
	else
	{
		adj += 3;
	}
	return levelGrid->getLocation(playerRow, playerCol + adj);
}

std::vector<Location> Enemy1::getPath()
{
	
	std::unordered_map<Location, Location, LocationHash, Equal> came_from;
	std::unordered_map<Location, int, LocationHash, Equal> cost_so_far;
	path_search(*levelGrid, currentSquare[0], currentGoal, came_from, cost_so_far);
	auto path = reconstruct_path(currentSquare[0], currentGoal, came_from);
	return path;
}

void Enemy1::doActions(SDL_Rect* camera, std::list<GameCharacter*> characters)
{
	punched = false;
	collision(characters);
	punched = player_->punching();
	otherLeft = player_->getX();
	otherRight = player_->getX() + player_->getWidth() / 2;

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
	manageSquareShift();
}