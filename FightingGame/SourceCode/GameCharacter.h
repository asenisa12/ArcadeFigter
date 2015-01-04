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
	static const int CHARACTER_SQUARES_NUM = 2;
	static const int UNUSED_SQUARES_NUM = 4;

	struct direction{ bool right; bool left; bool up; bool down; };
	struct Shift{ int X; int Y; } shifting;

	enum {LEFT, RIGHT, UP, DOWN};
	enum {PUNCHING, MOVING, PUNCHED, FALLING, STANDING};
	enum { GPLAYER, GENEMY};

	Location currentSquare[CHARACTER_SQUARES_NUM];
	direction moveDir;
	SquareGrid *levelGrid;
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
	
	int Row_;
	int Col_;
	
	int getCol(Location location);
	int getRow(Location location);
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
	GameCharacter(SquareGrid *grid, int characterType_,
		int screenW, int screenH);
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