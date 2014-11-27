#ifndef GAMECHARACTER
#define GAMECHARACTER

#include "GameObject.h"

class GameCharacter : public GameObject
{
protected:
	SDL_Rect * currentClip;
	SDL_RendererFlip flipType;
public:
	void renderCharacter(SDL_Renderer* gRenderer);
	int getBottomY();
};

#endif