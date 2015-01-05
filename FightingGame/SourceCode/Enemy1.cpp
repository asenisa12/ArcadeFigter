#include "Enemy1.h"

Enemy1::Enemy1(std::string path, Location startlocation, int screenW, int screenH,
	GameCharacter* player, SquareGrid *grid)
	:player_(player), action(false), GameCharacter(grid, GENEMY, screenW, screenH)
{

	health = 100;
	punchStart_count = 0;
	screenH_ = screenH;
	screenW_ = screenW;
	path_ = path;
	//movement speed == 0.5% from screen width
	movSpeed = screenW_*0.005;
	flipType = SDL_FLIP_NONE;
	posX_ = startlocation.X + squareSize() / 2;
	posY_ = startlocation.Y - squareSize() / 2;
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

bool Enemy1::moveToPosition(int X, int Y)
{
	return true;
}

void Enemy1::moving()
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
	Location goal = levelGrid->getLocation(playerRow, playerCol+adj);
	std::unordered_map<Location, Location, LocationHash, Equal> came_from;
	std::unordered_map<Location, int, LocationHash, Equal> cost_so_far;
	path_search(*levelGrid, { 620, 430 }, goal, came_from, cost_so_far);
	auto path = reconstruct_path({ 620, 430 }, goal, came_from);

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