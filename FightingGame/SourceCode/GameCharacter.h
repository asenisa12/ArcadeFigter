#ifndef GAMECHARACTER
#define GAMECHARACTER

#include "Grid.h"
#include "GameObject.h"
#include <list>
#include <cmath>
#include <vector>
#include <map>

using namespace grid;
enum { GPLAYER, GENEMY};

class GameCharacter : public GameObject
{
protected:
	static const int CHARACTER_SQUARES_NUM = 2;
	static const int UNUSED_SQUARES_NUM = 6;
	static const int DEATH_FRAMES = 50;

	struct direction{ bool right; bool left; bool up; bool down; };
	struct Shift{ int X; int Y; } shifting;

	enum {FIRST_SQUARE_ID=0, SECOND_SQUARE_ID=1};
	enum {LEFT, RIGHT, UP, DOWN};
	enum {PUNCHING, MOVING, PUNCHED, FALLING, STANDING};

	std::string animationName;
	std::vector < SDL_Rect >  Clips;
	enum{ WALLKING = 0, RUNNING = 1, JUMPING = 2, PUNCH = 3, FALLIN = 4 };
	std::vector<int> animFrameSize;
	std::map<std::string, std::vector < SDL_Rect >> animations;
	Location currentSquare[CHARACTER_SQUARES_NUM];
	direction moveDir;
	SquareGrid *levelGrid;
	SDL_Rect * currentClip;
	SDL_RendererFlip flipType;
	
	int CLIP_H;
	int CLIP_W;

	double movSpeed;
	int currentCondition;
	int health;
	int add;
	int framesToEnd;
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
	
	void changeSquare(int row, int col);
	void posToSquareMiddle();
	int getLocationCol(Location location);
	int getLocationRow(Location location);
	void changeCurrSquare(int dir);
	int squareSize();
	void manageSquareShift();
	bool characterInLeft(GameCharacter* character);
	bool characterInRigh(GameCharacter* character);
	void setGridAttributes(Location location);
	void animation(std::string name);
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void loadAnimation(std::string animNames[]);
	void loadClips(std::string name, int y, int endFrame);
	void resizeClips(SDL_Rect *clip);
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
	int getCol();
	int getRow();
	int getCondition();
	int CharacterType();
	SDL_RendererFlip getFlipType();
	bool ready_for_delete();
	virtual bool loadMedia(SDL_Renderer* gRenderer) = 0;
	virtual void update(std::list<GameCharacter*> characters) = 0;
};

#endif