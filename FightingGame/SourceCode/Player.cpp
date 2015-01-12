#include "Player.h"

Player::Player(std::string path, int screenW, int screenH, 
	Location startingLocation, SquareGrid *grid)
	: path_(path), GameCharacter(grid, GPLAYER, screenW, screenH, MAX_HEALTH)
{
	frame = 0;
	int add = 0;
	camera_pos = 1;
	//movement speed == 0.7% from screen w
	movSpeed =screenW_*0.007;
	objTexture = NULL;
	Row_ = getRow(startingLocation);
	Col_ = getCol(startingLocation);
	setGridAttributes(startingLocation);
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
	if (currentKeyStates[SDL_SCANCODE_LEFT] ||
		currentKeyStates[SDL_SCANCODE_RIGHT] ||
		currentKeyStates[SDL_SCANCODE_DOWN] ||
		currentKeyStates[SDL_SCANCODE_UP])
	{
		return true;
	}
	return false;
	
}

void Player::manageCameraPos(SDL_Rect* camera)
{
	//last x position on screen = 90% from screen W 
	if (posX_ > screenW_*(0.90) && camera_pos < 5){
		camera_pos++;
		Col_ = 1;
		shifting.X = 0;
		camera->x += screenW_;
		posX_ = posX_ - screenW_*(0.90);
	}
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
			return true;
		}
		else if (currentKeyStates[SDL_SCANCODE_Q])
		{
			if ((characterInLeft() && flipType == SDL_FLIP_HORIZONTAL) ||
				(characterInRigh() && flipType == SDL_FLIP_NONE))
			{
				enemy->editHealth(DAMAGE);
			}
		}
	}
	return false;
}

void Player::doActions(SDL_Rect* camera, std::list<GameCharacter*> characters)
{
	currentKeyStates = SDL_GetKeyboardState(NULL);
	collision(characters);
	bool ispunched = punched(characters);

	if (currentKeyStates[SDL_SCANCODE_SPACE] || jumping)
	{
		jump();
	}
	else if (ispunched)
	{
		fall();
	}
	else if (currentKeyStates[SDL_SCANCODE_Q] )
	{
		punch();

	}
	else
	{
		if (currentKeyStates[SDL_SCANCODE_LSHIFT])
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
			
	if (currentKeyStates[SDL_SCANCODE_RIGHT] && !currentKeyStates[SDL_SCANCODE_Q])
	{
		printf("posX: %d posY: %d Shift[X]: %d \n", posX_, getBottomY(), shifting.X);
		moveRight();
	}
	else if (currentKeyStates[SDL_SCANCODE_LEFT] && !currentKeyStates[SDL_SCANCODE_Q])
	{
		moveLeft();
	}
	else if (currentKeyStates[SDL_SCANCODE_UP] && !jumping)
	{
		moveUp();
	}
	else if (currentKeyStates[SDL_SCANCODE_DOWN] && !jumping)
	{
		moveDown();
	}
	
	//Cycle animation
	if (frame/4 >= lastclip )
	{
		frame = firstclip;
	}
	movSpeed = 3;

	manageSquareShift();
	manageCameraPos(camera);
	resizeClips(Clips);
}


void Player::changePosX(int changedX)
{
	posX_ = changedX;
}

