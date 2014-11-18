#include "Player.h"

Player::Player(std::string path,int screenW, int screenH)
	:path_(path), frame(0), flipType(SDL_FLIP_NONE), 
	screenH_(screenH), screenW_(screenW) 
{
	int add = 0;
	camera_pos = 1;
	//movement speed == 0.625% from screen w
	movSpeed =screenW_*0.00625;
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
	posX = (screenW_ - textureW) / 2; 
	posY = screenH_*(0.43);

	if (!LoadFromFile(path_.c_str(), gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		return false;
	}
	else
	{
		int x = 0;
		int y = 0;
		for (int i = 0; i < PUNCH_ANIMATION_FRAMES_END; i++)
		{

			if (i == WALKING_ANIMATION_FRAMES_END ||
				i==JUMPING_ANIMATION_FRAMES_END || i == RUNING_ANIMATION_FRAMES_END){
				x = 0;
				y += CLIP_H;
			}
			Clips[i].x = x;
			Clips[i].y = y;
			Clips[i].w = CLIP_W;
			Clips[i].h = CLIP_H;
			x += CLIP_W;
		}
	}
	return true;
}
bool Player::punch()
{
	if (currentKeyStates[SDL_SCANCODE_Q])
	{
		lastclip = PUNCH_ANIMATION_FRAMES_END;
		firstclip = JUMPING_ANIMATION_FRAMES_END * 4;
		if (frame / 4 < JUMPING_ANIMATION_FRAMES_END ||
			frame / 4 >= lastclip)
		{
			frame = firstclip;
		}
		frame++;
		return true;
	}
	return false;
}

bool Player::jump()
{
	if (currentKeyStates[SDL_SCANCODE_SPACE] || jumping)
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
				posY -= 10;
			}
			else
			{
				if (jumpH == 40){
					jumping = false;
					jumpH = 0;
				}
				posY += 10;
			}

		}
		if (jumpH % 3 == 0)
			frame++;
		return true;
	}
	return false;
}

bool Player::run()
{
	if (currentKeyStates[SDL_SCANCODE_LSHIFT])
	{
		firstclip = WALKING_ANIMATION_FRAMES_END * 4 + 1;
		lastclip = RUNING_ANIMATION_FRAMES_END;
		if (frame / 4 < WALKING_ANIMATION_FRAMES_END){
			frame = firstclip;
		}
		movSpeed = 10;
		return true;
	}
	return false;
}

bool Player::moveRight(bool punching)
{
	if (currentKeyStates[SDL_SCANCODE_RIGHT] && !punching)
	{
		//max x = 92% from screen w
		if (posX < (screenW_*(0.92)))
			posX += movSpeed;
		flipType = SDL_FLIP_NONE;
		return true;
	}
	return false;
}

bool Player::moveLeft(bool punching)
{
	if (currentKeyStates[SDL_SCANCODE_LEFT] && !punching)
	{
		if (posX > 3)
			posX -= movSpeed;
		flipType = SDL_FLIP_HORIZONTAL;
		return true;
	}
	return false;
}

bool Player::moveUp()
{
	if (currentKeyStates[SDL_SCANCODE_UP] && !jumping)
	{
		//max y = 42% form screen h
		if (posY > (screenH_*(0.42))){
			posY -= 2;
			add--;
			movSpeed -= 0.05;
		}
		return true;
	}
	return false;
}

bool Player::moveDown()
{
	if (currentKeyStates[SDL_SCANCODE_DOWN] && !jumping)
	{
		//min y = 54% form screen h
		if (posY < (screenH_*(0.54)))
		{
			posY += 2;
			add++;
			movSpeed += 0.05;
		}
		return true;
	}
	return false;
}

bool Player::checkKeys()
{
	if (currentKeyStates[SDL_SCANCODE_LEFT] || (currentKeyStates[SDL_SCANCODE_RIGHT])
		|| currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_UP])
		return true;
	return false;
	
}

void Player::manageCameraPos(SDL_Rect* camera)
{
	//last x position on screen = 90% from screen W 
	if (posX > screenW_*(0.90) && camera_pos < 5){
		camera_pos++;
		camera->x += screenW_;
		posX = posX - screenW_*(0.90);
	}
}

void Player::resizeClips()
{
	currentClip = &Clips[frame / 4];
	//the height or width of the texture = clip w or h * 0.3% or 0.32% 
	//+ additional x or y pos if the charracter gets closer or away
	textureW = (currentClip->w * (0.0030) *screenW_) + add;
	textureH = (currentClip->h * (0.0032) *screenH_) + add;
}

void Player::doActions(SDL_Event e, SDL_Rect* camera)
{
	currentKeyStates = SDL_GetKeyboardState(NULL);
	bool punching = false;
	if (!jump())
	{
		if (!(punching = punch()))
		{
			if (!run())
			{
				lastclip = WALKING_ANIMATION_FRAMES_END;
				firstclip = 5;
			}
			if (checkKeys())
			{
				frame++;
			}
			else
			{
				frame = 0;
			}

		}
	}
			
	if (!moveRight(punching))
	{
		if (!moveLeft(punching))
		{
			if (!moveUp())
			{
				moveDown();
			}
		}
	}
	
	//Cycle animation
	if (frame/4 >= lastclip )
	{
		frame = firstclip;
	}
	movSpeed = 3;

	manageCameraPos(camera);
	resizeClips();
}

void Player::renderPlayer(SDL_Renderer* gRenderer)
{
	//Render current frame
	render(posX, posY, currentClip, 0, NULL, flipType, gRenderer, textureW, textureH);

}

void Player::changePosX(int changedX)
{
	posX = changedX;
}

int Player::getX()
{
	return posX;
}

int Player::getY()
{
	return posY;
}