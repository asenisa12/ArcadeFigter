#ifndef ENEMY_ONE
#define ENEMY_ONE

#include "GameCharacter.h"
#include "PathFinding.h"
#include <string>
#include <future>

enum{ FERRIS };

class Enemy1 : public GameCharacter
{

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

	GameCharacter* player_;
	Location lastPlayerSquare;
	Location currentGoal;
	Location PrevSquare;
	std::vector<Location> path;

	void findDestination();
	void moveToPosition(int X, int Y);
	void moving();
	void punch();
	void fall();
	Location getPlayerSquare();
	std::string path_;
	std::vector<Location> getPath();
	SDL_Rect *Clips;
public:
	Enemy1(std::string path, Location startlocation, int screenW, int screenH, 
		GameCharacter* player, SquareGrid *grid, int id);
	~Enemy1();
	void loadData(std::string path);
	bool loadMedia(SDL_Renderer* renderer);
	void doActions(SDL_Rect* camera, std::list<GameCharacter*> characters);
};

#endif // !ENEMY_ONE
