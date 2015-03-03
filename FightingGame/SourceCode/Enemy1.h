#ifndef ENEMY
#define ENEMY

#include "GameCharacter.h"
#include "PathFinding.h"
#include <string>
#include <tuple>
#include <future>

//The ids of the enemies are:
//PUNK=0
//THUG=1
//FAT THUG=2
//FAT BASTARD=3
//RAPTOR=4
//BUTCHER=5
//PRIVATE=6
//CORPORAL=7
//SERGEANT=8
//CYBORG=9
//MONSTER=10

enum{ FERRIS =0 };
enum{TARGET=0, AVAILABLE=1};
extern std::vector < std::tuple<GameCharacter*, bool> > players;

typedef std::list<std::tuple<GameCharacter*, bool>>::iterator playerIter;

class Enemy1 : public GameCharacter
{
	int playersCOUNT;
	int enemyID;
	int WALKING_ANIMATION_END;
	int PUNCHING_ANIMATION_END;
	int FALLING_ANIMATION_END;

	double SHIFTING_PERCENTIGE;
	double DIFF_BY_X_PERCENTIGE;
	double DIFF_BY_Y_PERCENTIGE;

	int MAX_HEALTH;
	int DAMAGE;
	int FALLING_CLIP_W;

	int destX;
	int destY;
	int diffrenceY;
	int diffrenceX;
	int punchStart_count;
	bool other_frame;
	bool punched;
	bool action;
	Mix_Chunk *punchSound = NULL;
	Mix_Chunk *wooshSound = NULL;

	Location lastPlayerSquare;
	Location currentGoal;
	Location PrevSquare;
	std::vector<Location> path;

	//finds the next destination square and optimises the path
	void findDestination();
	void moveToPosition(int X, int Y);

	void moving();
	void punch();
	void fall();

	//checks if the player is punching
	bool player_punching();
	void punch_players();
	void doActions(std::list<GameCharacter*> characters);
	Location getGoalSquare();
	std::string path_;
	bool synchroniseFlip(GameCharacter* player);
	std::vector<Location> getPath();
public:
	Enemy1(std::string path, Location startlocation, int screenW, int screenH, SquareGrid *grid, int id);
	~Enemy1();
	void loadData(std::string path);
	bool loadMedia(SDL_Renderer* renderer);
	void update(std::list<GameCharacter*> characters);
};

#endif
