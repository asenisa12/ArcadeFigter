#ifndef ENEMY_ONE
#define ENEMY_ONE

#include "GameCharacter.h"
#include "PathFinding.h"
#include <string>
#include <tuple>
#include <future>

enum{ FERRIS };
enum{TARGET=0, AVAILABLE=1};
extern std::vector < std::tuple<GameCharacter*, bool> > players;

typedef std::list<std::tuple<GameCharacter*, bool>>::iterator playerIter;

class Enemy1 : public GameCharacter
{
	int playersCOUNT;
	int playernum;
	int enemyID;
	int WALKING_ANIMATION_END;
	int PUNCHING_ANIMATION_END;
	int FALLING_ANIMATION_END;

	double SHIFTING_PERCENTIGE;
	double DIFF_BY_X_PERCENTIGE;
	double DIFF_BY_Y_PERCENTIGE;

	int MAX_HEALTH;
	int DAMAGE;
	int CLIP_H;
	int CLIP_W;
	int FALLING_CLIP_W;

	int destX;
	int destY;
	int diffrenceY;
	int diffrenceX;
	int punchStart_count;
	bool other_frame;
	bool punched;
	bool action;

	GameCharacter* target_;
	Location lastPlayerSquare;
	Location currentGoal;
	Location PrevSquare;
	std::vector<Location> path;

	void findDestination();
	void getTarget();
	void moveToPosition(int X, int Y);
	void moving();
	void punch();
	void fall();
	bool player_punching();
	void punch_players();
	void doActions(std::list<GameCharacter*> characters);
	Location getGoalSquare();
	std::string path_;
	std::vector<Location> getPath();
	SDL_Rect *Clips;
public:
	Enemy1(std::string path, Location startlocation, int screenW, int screenH, SquareGrid *grid, int id);
	~Enemy1();
	void loadData(std::string path);
	bool loadMedia(SDL_Renderer* renderer);
	void update(std::list<GameCharacter*> characters);
};

#endif // !ENEMY_ONE
