#include "Player.h"

Player::Player(std::string path,int screenW, int screenH)
	:path_(path) 	
{
	characterType = GPLAYER;
	health = 200;
	currentCondition = STANDING;
	frame = 0;
	screenH_=screenH; 
	screenW_ = screenW;
	flipType = SDL_FLIP_NONE;
	int add = 0;
	camera_pos = 1;
	//movement speed == 0.7% from screen w
	movSpeed =screenW_*0.007;
	objTexture = NULL;
}

Player::~Player()
{
	free();
}

bool Player::loadMedia(SDL_Renderer* gRenderer)
{
	/*textureH = screenH_*(0.26); 
	textureW = screenW_*(0.17);*/
	posX_ = (screenW_ - mWidth) / 2; 
	posY_ = screenH_*(0.43);

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
			i==JUMPING_ANIMATION_FRAMES_END || 
			i == RUNING_ANIMATION_FRAMES_END ||
			i== PUNCH_ANIMATION_FRAMES_END){
			x = 0;
			y += CLIP_H;
		}
		Clips[i].x = x;
		Clips[i].y = y;
		Clips[i].w = CLIP_W;
		Clips[i].h = CLIP_H;
		x += CLIP_W;
	}

	return true;
}

void Player::punch()
{
	currentCondition = PUNCHING;
	animation(PUNCH_ANIMATION_FRAMES_END, JUMPING_ANIMATION_FRAMES_END);
}

void Player::fall()
{
	currentCondition = PUNCHED;
	animation(FALLING_ANIMATION_FRAMES_END, PUNCH_ANIMATION_FRAMES_END);
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

bool Player::checkKeys()
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
		camera->x += screenW_;
		posX_ = posX_ - screenW_*(0.90);
	}
}

bool Player::punched(std::vector<GameCharacter*> characters)
{
	for (std::vector<GameCharacter*>::iterator it = characters.begin(); it != characters.end(); ++it)
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

void Player::doActions(SDL_Rect* camera, std::vector<GameCharacter*> characters)
{
	currentKeyStates = SDL_GetKeyboardState(NULL);
	bool punching = false;
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
		if (checkKeys())
		{
			frame++;
		}
		else
		{
			currentCondition = STANDING;
			frame = 0;
		}
	}
			
	if (currentKeyStates[SDL_SCANCODE_RIGHT] && !punching)
	{
		moveRight();
	}
	else if (currentKeyStates[SDL_SCANCODE_LEFT] && !punching)
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

	manageCameraPos(camera);
	resizeClips(Clips);
}


void Player::changePosX(int changedX)
{
	posX_ = changedX;
}

