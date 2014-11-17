#include "Player.h"

Player::Player(std::string path, SDL_Renderer* renderer,int screenW, int screenH)
	:path_(path), gRenderer(renderer), frame(0), flipType(SDL_FLIP_NONE), 
	screenH_(screenH), screenW_(screenW) 
{
	int add = 0;
	camera_pos = 1;
	movSpeed =screenW_*0.00625;
	objTexture = NULL;
}

Player::~Player()
{
	free();
}

bool Player::loadMedia()
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
bool Player::punch(const Uint8* currentKeyStates)
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

bool Player::jump(const Uint8* currentKeyStates)
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

bool Player::run(const Uint8* currentKeyStates)
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

void Player::doActions(SDL_Event e, SDL_Rect* camera)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	if (!jump(currentKeyStates))
	{
		if (!punch(currentKeyStates))
		{
			if (!run(currentKeyStates))
			{
				lastclip = WALKING_ANIMATION_FRAMES_END;
				firstclip = 5;
			}
			if (currentKeyStates[SDL_SCANCODE_LEFT] || (currentKeyStates[SDL_SCANCODE_RIGHT])
				|| currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_UP])
			{
				frame++;
			}
			else
			{
				frame = 0;
			}

		}
	}
			
	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		if (posX > 3)
			posX -= movSpeed;
		flipType = SDL_FLIP_HORIZONTAL;
	}
	else if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		if (posX < (screenW_*(0.92)))
			posX += movSpeed;
		flipType = SDL_FLIP_NONE;
	}
	else if (currentKeyStates[SDL_SCANCODE_UP] && !jumping)
	{
		if (posY > (screenH_*(0.42))){
			posY -= 2;
			add--;
			movSpeed -= 0.05;
		}
	}
	else if (currentKeyStates[SDL_SCANCODE_DOWN] && !jumping)
	{
		if (posY < (screenH_*(0.54)))
		{
			posY += 2;
			add++;
			movSpeed += 0.05;
		}
	}

	if (frame/4 >= lastclip )
	{
		frame = firstclip;
	}
	movSpeed = 3;
	//Cycle animation



	if (posX > screenW_*(0.90) && camera_pos < 5){
		camera_pos++;
		camera->x += screenW_;
		posX = posX - screenW_*(0.90);
	}

	currentClip = &Clips[frame / 4];
	textureW = (currentClip->w * (0.0030) *screenW_) + add;
	textureH = (currentClip->h * (0.0032) *screenH_) + add;
}

void Player::renderPlayer()
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