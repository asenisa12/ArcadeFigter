#ifndef PLAYER
#define PLAYER

#include "GameCharacter.h"
#include <cmath> 
#include <map>
#include <iostream>

enum {Player1=1, Player2=2};

struct pEvent
{
	bool runPunch;
	bool normalPunch;
	bool superPunch;
	bool moveLeft;
	bool moveRight;
	bool moveUP;
	bool moveDown;
	bool grab;
	bool jump;
	bool run;
};

struct pKeys
{
	int moveR;
	int moveL;
	int moveU;
	int moveD;
	int grab;
	int punch;
	int run;
	int jump;
};



class Player : public GameCharacter
{	
	int add;
	int camera_pos;
	int jumpH;
	int playerID;
	bool jumping;
	const Uint8* currentKeyStates;
	pEvent playerEvent;

	int WALKING_ANIMATION_FRAMES_END;
	int RUNING_ANIMATION_FRAMES_END;
	int JUMPING_ANIMATION_FRAMES_END;
	int PUNCH_ANIMATION_FRAMES_END;
	int FALLING_ANIMATION_FRAMES_END;

	bool sPunch;
	bool rPunch;
	bool grabing;
	int MAX_HEALTH;
	int DAMAGE;

	//SDL_Rect *Clips;
	
	
	std::string path_;
	std::list<std::string> combos;
	bool punched(std::list<GameCharacter*> characters);
	void fall();
	void continuingAnim(bool &var, std::string name);
	void superPunch();
	void runPunch();
	void grab();
	void punch();
	void jump();
	void run();
	void checkCondition();
	bool checkMoveKeys();
	void push_event(std::string name);
	void loadData(std::string path);

	void doActions(std::list<GameCharacter*> characters);

public:
	Player(std::string jsonPath, int screenW, int screenH, SquareGrid *grid, int player);
	~Player();
	bool loadMedia(SDL_Renderer* gRenderer);
	//implements key presses
	void handleEvent(pKeys playerKey);
	void changePosX(int changedX);
	void manageCameraPos();
	void update(std::list<GameCharacter*> characters);
};

#endif