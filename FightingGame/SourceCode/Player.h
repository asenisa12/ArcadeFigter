#ifndef PLAYER
#define PLAYER

#include "GameObject.h"

class Player : public GameObject
{	
	SDL_RendererFlip flipType;
	int posX, posY;
	int frame;
	static const int WALKING_ANIMATION_FRAMES = 7;

	SDL_Rect walkingClips[WALKING_ANIMATION_FRAMES];
	SDL_Renderer* gRenderer;
	std::string path_;
public:
	Player(std::string path, SDL_Renderer* renderer);
	~Player();
	bool loadMedia();
	void doActions(SDL_Event e);
	void renderPlayer();
	int getX();
	int getY();
};

#endif