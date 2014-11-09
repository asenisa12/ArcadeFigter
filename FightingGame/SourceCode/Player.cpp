#include "Player.h"

Player::Player(std::string path, SDL_Renderer* renderer)
:path_(path), gRenderer(renderer), frame(0), flipType(SDL_FLIP_NONE),
posX((640 - 75) / 2), posY((480 - 96) / 2)
{
	objTexture = NULL;
}

Player::~Player()
{
	free();
}

bool Player::loadMedia()
{
	bool success = true;

	if (!LoadFromFile(path_.c_str(),gRenderer))
	{
		printf("Failed to load walking animation texture!\n");
		success = false;
	}
	else
	{
		int x = 0;
		int y = 0;
		for (int i = 0; i < 7; i++)
		{
			walkingClips[i].x = x;
			walkingClips[i].y = y;
			walkingClips[i].w = 75;
			walkingClips[i].h = 92;
			x += 75;
		}
	}
	return success;
}

void Player::doActions(SDL_Event e)
{

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		if (posX>3)
			posX -= 3;
		flipType = SDL_FLIP_HORIZONTAL;
	}
	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		if (posX<3065)
			posX += 3;
		flipType = SDL_FLIP_NONE;
	}
	if (currentKeyStates[SDL_SCANCODE_UP])
	{
		if (((480 - 92) / 2) < posY)
			posY -= 2;
	}
	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		if (posY<360)
			posY += 2;
	}

	if (e.type == SDL_KEYDOWN)
		frame++;

	//Cycle animation
	if (frame / 4 >= WALKING_ANIMATION_FRAMES)
	{
		frame = 0;
	}
}

void Player::renderPlayer()
{
	//Render current frame
	SDL_Rect* currentClip = &walkingClips[frame / 4];
	render(posX, posY, currentClip, 0, NULL, flipType, gRenderer);

}

int Player::getX()
{
	return posX;
}

int Player::getY()
{
	return posY;
}