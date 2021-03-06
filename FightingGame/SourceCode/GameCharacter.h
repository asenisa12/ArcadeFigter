#ifndef GAMECHARACTER
#define GAMECHARACTER

#include "Grid.h"
#include "GameObject.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
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
	enum {PUNCHING, MOVING, PUNCHED, FALLING, STANDING, JUMPING};

	std::string animationName;
	std::vector < SDL_Rect >  Clips;

	std::vector<int> animFrameSize;
	std::map<std::string, std::vector < SDL_Rect >> animations;
	Location currentSquare[CHARACTER_SQUARES_NUM];
	direction moveDir;
	SquareGrid *levelGrid;
	SDL_Rect * currentClip;
	SDL_RendererFlip flipType;
	GameCharacter* target_;
	
	int CLIP_H;
	int CLIP_W;

	double movSpeed;
	int playernum;
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
	int playSound(Mix_Chunk* chunk);
	Mix_Chunk* loadWAV(std::string name);
	void loadAnimation(std::string animNames[]);
	void loadClips(std::string name, int y, int endFrame);
	void resizeClips(SDL_Rect *clip);
	void collision(std::list<GameCharacter*> characters);
public:
	GameCharacter* getTarget();
	void setTarget(GameCharacter* target, int num);
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