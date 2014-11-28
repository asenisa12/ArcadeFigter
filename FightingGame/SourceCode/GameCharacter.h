#ifndef GAMECHARACTER
#define GAMECHARACTER

#include "GameObject.h"
#include <cmath>  

class GameCharacter : public GameObject
{
protected:
	struct direction{ bool right; bool left; bool up; bool down; };
	struct direction moveDir;
	SDL_Rect * currentClip;
	SDL_RendererFlip flipType;
	
	double movSpeed;
	int add;
	int frame;
	int screenH_, screenW_;

	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void resizeClips(SDL_Rect Clips[]);
	void collision(GameCharacter* enemy[], int charactersCount);
public:
	void renderCharacter(SDL_Renderer* gRenderer);
	int getBottomY();
};

#endif