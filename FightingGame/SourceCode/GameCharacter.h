#ifndef GAMECHARACTER
#define GAMECHARACTER

#include "GameObject.h"
#include <vector>
#include <cmath>  

class GameCharacter : public GameObject
{
protected:
	struct direction{ bool right; bool left; bool up; bool down; };
	struct direction moveDir;
	enum {PUNCHING, MOVING, PUNCHED, FALLING, STANDING};
	enum { GPLAYER, GENEMY};
	SDL_Rect * currentClip;
	SDL_RendererFlip flipType;
	
	double movSpeed;
	int currentCondition;
	int health;
	int add;
	int frame;
	int screenH_, screenW_;
	int firstclip;
	int lastclip;
	int characterType;
	
	int otherBottom;
	int otherLeft;
	int otherRight;

	int myBottom;
	int myLeft;
	int myRight;

	bool characterInLeft();
	bool characterInRigh();
	void animation(int last, int first);
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void resizeClips(SDL_Rect Clips[]);
	void collision(std::vector<GameCharacter*> characters);
public:
	bool punching();
	void renderCharacter(SDL_Renderer* gRenderer);
	int getBottomY();
	void editHealth(int damage);
	int getHealth();
	int getCondition();
	int CharacterType();
	virtual void doActions(SDL_Rect* camera, std::vector<GameCharacter*> characters) = 0;
};

#endif