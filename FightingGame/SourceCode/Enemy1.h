#ifndef ENEMY_ONE
#define ENEMY_ONE

#include "GameObject.h"
#include <string>

class Enemy1 : public GameObject
{
public:
	Enemy1(std::string path, SDL_Renderer* renderer);
	~Enemy1();

private:

};




#endif // !ENEMY_ONE
