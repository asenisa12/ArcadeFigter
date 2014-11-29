#ifndef GAMECHARACTER
#define GAMECHARACTER

#include "GameObject.h"
#include <cmath>  

class GameCharacter : public GameObject
{
protected:
	struct direction{ bool right; bool left; bool up; bool down; };
	struct direction moveDir;
	struct condition{ bool punching; bool moving; bool punched; bool standing; };
	struct condition currentCondition;
	SDL_Rect * currentClip;
	SDL_RendererFlip flipType;
	
	double movSpeed;
	int add;
	int frame;
	int screenH_, screenW_;
	int firstclip;
	int lastclip;
	int enemyBottom;
	int enemyLeft;
	int enemyRight;

	int playerBottom;
	int playerLeft;
	int playerRight;

	bool characterInLeft();
	bool characterInRigh();
	void animation(int last, int first);
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void resizeClips(SDL_Rect Clips[]);
	void collision(GameCharacter* enemy[], int charactersCount);
public:
	bool punching();
	void renderCharacter(SDL_Renderer* gRenderer);
	int getBottomY();
	condition getCondition();
};

#endif