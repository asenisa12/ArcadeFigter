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
		animFrameSize.push_back(WALKING_ANIMATION_FRAMES_END);
		animFrameSize.push_back(RUNING_ANIMATION_FRAMES_END);
		animFrameSize.push_back(JUMPING_ANIMATION_FRAMES_END);
		animFrameSize.push_back(PUNCH_ANIMATION_FRAMES_END);
		animFrameSize.push_back(FALLING_ANIMATION_FRAMES_END);
		MAX_HEALTH = consts[U("MAX_HEALTH")].as_integer();
		DAMAGE = consts[U("DAMAGE")].as_integer();
		CLIP_H = consts[U("CLIP_H")].as_integer();
		CLIP_W = consts[U("CLIP_W")].as_integer();
		health = MAX_HEALTH;

	}
	else
	{
		printf("Error: Can't open file %s\n",path.c_str());
	}
	jsonFile.close();
}

Player::~Player()
{
	free();
}

bool Player::loadMedia(SDL_Renderer* gRenderer)
{

	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	std::string animNames[] = { "WALLKING", "RUNNING", "JUMPING", "PUNCH", "FALLING" };
	loadAnimation(animNames);
	posToSquareMiddle();
	return true;
}


void Player::fall()
{
	currentCondition = PUNCHED;
	animation("FALLING");
	frame++;
}

void Player::punch()
{
	currentCondition = PUNCHING;
	animation("PUNCH");
	frame++;
}

void Player::jump()
{
	if (animationName != "JUMPING" || !jumping){
		firstclip = 0;
		Clips = animations["JUMPING"];
		frame = firstclip;
		jumping = true;
		animationName = "JUMPING";
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
	animation("RUNNING");
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
				if (frame / 4 == Clips.size()- 1)
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
			animation("WALLKING");
			firstclip = 5;
		}
		//checks if the moving kes are pressed
		if (checkMoveKeys())
		{
			frame++;
		}
		else
		{
			
			animation("WALLKING");
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
		//printf("posX: %d posY: %d Shift[X]: %d \n", posX_, /*getBottomY()*/posY_, shifting.X);
		moveUp();
	}
	else if (playerEvent.moveDown && !jumping)
	{
		moveDown();
	}

	//Cycle animation
	if (frame / 4 >= Clips.size())
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
		animation("FALLING");
		frame = 4 * 4;
	}
	resizeClips(&Clips[frame/4]);
}


void Player::changePosX(int changedX)
{
	posX_ = changedX;
}

