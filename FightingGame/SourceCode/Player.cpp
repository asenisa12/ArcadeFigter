#include "Player.h"

Player::Player(std::string jsonPath, int screenW, int screenH, SquareGrid *grid, int player)
	:playerID(player), GameCharacter(grid, GPLAYER, screenW, screenH)
{
	frame = 0;
	int add = 0;
	camera_pos = 1;

	loadData(jsonPath);
	//movement speed == 0.7% from screen w
	movSpeed =screenW_*0.007;
	objTexture = NULL;
	setGridAttributes(grid->getLocation(Row_, Col_));
}

void Player::loadData(std::string path)
{

	std::fstream jsonFile;
	jsonFile.open(path);
	if (jsonFile.is_open())
	{

		jsonObj player = jsonObj::parse(jsonFile);
		jsonObj attr;
		if (playerID == Player1) attr = player[U("Player1")];
		else if (playerID == Player2) attr = player[U("Player2")];
	
		path_ = utility::conversions::to_utf8string(attr[U("texture")].as_string());
		Row_ = attr[U("row")].as_integer();
		Col_ = attr[U("col")].as_integer();

		jsonObj consts = player[U("Constants")];
		WALKING_ANIMATION_FRAMES_END = consts[U("WALKING_ANIMATION_FRAMES_END")].as_integer();
		RUNING_ANIMATION_FRAMES_END = consts[U("RUNING_ANIMATION_FRAMES_END")].as_integer();
		JUMPING_ANIMATION_FRAMES_END = consts[U("JUMPING_ANIMATION_FRAMES_END")].as_integer();
		PUNCH_ANIMATION_FRAMES_END = consts[U("PUNCH_ANIMATION_FRAMES_END")].as_integer();
		FALLING_ANIMATION_FRAMES_END = consts[U("FALLING_ANIMATION_FRAMES_END")].as_integer();
		MAX_HEALTH = consts[U("MAX_HEALTH")].as_integer();
		DAMAGE = consts[U("DAMAGE")].as_integer();
		CLIP_H = consts[U("CLIP_H")].as_integer();
		CLIP_W = consts[U("CLIP_W")].as_integer();
		health = MAX_HEALTH;
		Clips = new SDL_Rect[FALLING_ANIMATION_FRAMES_END];

	}
	else
	{
		printf("Error: Can't open file %s\n",path.c_str());
	}
	jsonFile.close();
}

Player::~Player()
{
	delete[] Clips;
	free();
}

bool Player::loadMedia(SDL_Renderer* gRenderer)
{

	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	int x = 0;
	int y = 0;
	for (int i = 0; i < FALLING_ANIMATION_FRAMES_END; i++)
	{

		if (i == WALKING_ANIMATION_FRAMES_END ||
			i == JUMPING_ANIMATION_FRAMES_END || 
			i == RUNING_ANIMATION_FRAMES_END ||
			i == PUNCH_ANIMATION_FRAMES_END )
		{
			x = 0;
			y += CLIP_H;
		}
		Clips[i] = { x, y, CLIP_W, CLIP_H };
		x += CLIP_W;
	}
	resizeClips(Clips);
	posToSquareMiddle();
	return true;
}


void Player::fall()
{
	currentCondition = PUNCHED;
	animation(FALLING_ANIMATION_FRAMES_END, PUNCH_ANIMATION_FRAMES_END);
}

void Player::punch()
{
	currentCondition = PUNCHING;
	animation(PUNCH_ANIMATION_FRAMES_END, JUMPING_ANIMATION_FRAMES_END);
}

void Player::jump()
{
	firstclip = RUNING_ANIMATION_FRAMES_END * 4;
	lastclip = JUMPING_ANIMATION_FRAMES_END;
	if (frame / 4 < RUNING_ANIMATION_FRAMES_END || !jumping)
	{
		frame = firstclip;
		jumping = true;
	}

	if (jumping)
	{
		jumpH++;
		if (jumpH <= 20){
			posY_ -= 10;
		}
		else
		{
			if (jumpH == 40){
				jumping = false;
				jumpH = 0;
			}
			posY_ += 10;
		}

	}
	if (jumpH % 3 == 0)
		frame++;
}

void Player::run()
{
	
	firstclip = WALKING_ANIMATION_FRAMES_END * 4 + 1;
	lastclip = RUNING_ANIMATION_FRAMES_END;
	if (frame / 4 < WALKING_ANIMATION_FRAMES_END){
		frame = firstclip;
	}
	movSpeed = 10;

}

bool Player::checkMoveKeys()
{
	if (playerEvent.moveLeft || playerEvent.moveRight ||
		playerEvent.moveUP || playerEvent.moveDown)
	{
		return true;
	}
	return false;
	
}

void Player::manageCameraPos()
{
	Col_ = 0;
	changeSquare(Row_, Col_);
	posX_ = currentSquare[FIRST_SQUARE_ID].X + squareSize() / 2;
	shifting.X = 0;
}

bool Player::punched(std::list<GameCharacter*> characters)
{
	for (std::list<GameCharacter*>::iterator it = characters.begin(); it != characters.end(); ++it)
	{
		if ((*it)->CharacterType() == GPLAYER)
			continue;

		GameCharacter *enemy = (*it);
		otherLeft = enemy->getX();
		otherRight = enemy->getX() + enemy->getWidth() / 2;

		myBottom = posY_ + mHigth;
		myLeft = posX_ + mWidth / 5;

		if (enemy->punching() && enemy->getCondition()!=PUNCHED)
		{
			if ((characterInLeft(enemy) || characterInRigh(enemy)) &&
				enemy->getRow() == Row_)
			{
				if (otherLeft > myLeft) flipType == SDL_FLIP_HORIZONTAL;
				if (otherLeft < myLeft) flipType == SDL_FLIP_NONE;
				return true;
			}
		}
		else if (playerEvent.normalPunch)
		{
			if ((characterInLeft(enemy) && flipType == SDL_FLIP_HORIZONTAL) ||
				(characterInRigh(enemy) && flipType == SDL_FLIP_NONE))
			{
				if (frame / 4 == PUNCH_ANIMATION_FRAMES_END - 1)
				{
					printf("DAMAGE\n");
					enemy->editHealth(DAMAGE);
				}
			}
		}
	}
	return false;
}

void Player::handleEvent(pKeys playerKey)
{
	playerEvent = { false, false, false, false, false, false, false, false, false };
	auto currentKeyStates = SDL_GetKeyboardState(NULL);

	if (currentKeyStates[playerKey.punch] && currentKeyStates[playerKey.jump])
	{
		playerEvent.superPunch = true;
	}
	else if (currentKeyStates[playerKey.punch] && currentKeyStates[playerKey.run])
	{
		playerEvent.runPunch = true;
	}
	else if (currentKeyStates[playerKey.punch])
	{
		playerEvent.normalPunch = true;
	}
	if (currentKeyStates[playerKey.jump])
	{
		playerEvent.jump = true;
	}
	if (currentKeyStates[playerKey.run])
	{
		playerEvent.run = true;
	}
	if (currentKeyStates[playerKey.moveD])
	{
		playerEvent.moveDown = true;
	}
	if (currentKeyStates[playerKey.moveU])
	{
		playerEvent.moveUP = true;
	}
	if (currentKeyStates[playerKey.moveL])
	{
		playerEvent.moveLeft = true;
	}
	if (currentKeyStates[playerKey.moveR])
	{
		playerEvent.moveRight = true;
	}

}

void Player::doActions(std::list<GameCharacter*> characters)
{
	collision(characters);
	bool ispunched = punched(characters);

	if (playerEvent.jump || jumping)
	{
		jump();
	}
	else if (currentCondition == PUNCHED)
	{
		fall();
	}
	else if (playerEvent.normalPunch)
	{
		punch();

	}
	else
	{
		if (playerEvent.run)
		{
			run();
		}
		else
		{
			lastclip = WALKING_ANIMATION_FRAMES_END;
			firstclip = 5;
		}
		//checks if the moving kes are pressed
		if (checkMoveKeys())
		{
			frame++;
		}
		else
		{
			currentCondition = STANDING;
			frame = 0;
		}
	}

	if (playerEvent.moveRight && !playerEvent.normalPunch)
	{
		//printf("posX: %d posY: %d Shift[X]: %d \n", posX_, /*getBottomY()*/posY_, shifting.X);
		moveRight();
	}
	else if (playerEvent.moveLeft && !playerEvent.normalPunch)
	{
		moveLeft();
	}
	else if (playerEvent.moveUP && !jumping)
	{
		printf("posX: %d posY: %d Shift[X]: %d \n", posX_, /*getBottomY()*/posY_, shifting.X);
		moveUp();
	}
	else if (playerEvent.moveDown && !jumping)
	{
		moveDown();
	}

	//Cycle animation
	if (frame / 4 >= lastclip)
	{
		frame = firstclip;
	}
	movSpeed = 3;

	manageSquareShift();
}

void Player::update(std::list<GameCharacter*> characters)
{
	if (health > 0)
	{
		doActions(characters);
	}
	else
	{
		framesToEnd--;
		frame = (PUNCH_ANIMATION_FRAMES_END + 4)*4;
	}
	resizeClips(Clips);
}


void Player::changePosX(int changedX)
{
	posX_ = changedX;
}

