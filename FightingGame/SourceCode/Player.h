#ifndef PLAYER
#define PLAYER

#include "GameObject.h"

class Player : public GameObject
{	
	SDL_RendererFlip flipType;
	int posX, posY;
	double textureW, textureH;
	int frame;
	int screenH_, screenW_;
	double movSpeed;
	static const int WALKING_ANIMATION_FRAMES = 7;

	SDL_Rect walkingClips[WALKING_ANIMATION_FRAMES];
	SDL_Renderer* gRenderer;
	std::string path_;
public:
	Player(std::string path, SDL_Renderer* renderer, int screenH, int screenW);
	~Player();
	bool loadMedia();
	void doActions(SDL_Event e);
	void renderPlayer();
	void changePosX(int changedX);
	int getX();
	int getY();
};

#endif