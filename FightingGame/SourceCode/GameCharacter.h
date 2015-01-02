#ifndef GAMECHARACTER
#define GAMECHARACTER

#include "Grid.h"
#include "GameObject.h"
#include <list>
#include <cmath>
#include <vector>

using namespace grid;

class GameCharacter : public GameObject
{
protected:
	struct direction{ bool right; bool left; bool up; bool down; };
	struct Neighbors
	{
		Location UPL;
		Location UPR;
		Location DownL;
		Location DownR;
		Location Left;
		Location Right;
	};
	struct Shift{ int X; int Y; } shifting;
	Location currentSquare[2];
	direction moveDir;
	Neighbors neighbors;
	SquareGrid *levelGrid;
	enum {LEFT, RIGHT, UP, DOWN};
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
		
	Neighbors getNeighbors();
	void changeCurrSquare(int dir);
	int squareSize();
	void manageSquareShift();
	bool characterInLeft();
	bool characterInRigh();
	void setGridAttributes(Location location);
	void animation(int last, int first);
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void resizeClips(SDL_Rect Clips[]);
	void collision(std::list<GameCharacter*> characters);
public:
	bool punching();
	void renderCharacter(SDL_Renderer* gRenderer);
	int getBottomY();
	void editHealth(int damage);
	Location* getCurrSquare();
	int getHealth();
	int getCondition();
	int CharacterType();
	virtual void doActions(SDL_Rect* camera, std::list<GameCharacter*> characters) = 0;
};

#endif