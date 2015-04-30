#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include <fstream>
#include <vector>

using namespace rapidjson;

static const int FRAMES_DELIMITOR = 4;

class GameObject
{	
protected:
	int posX_, posY_;
	double mWidth;
	double mHigth;
	SDL_Texture* objTexture;
public:
	bool LoadFromFile(std::string path, SDL_Renderer* gRenderer);
	void free();
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blend);
	void setAlpha(Uint8 alpha);
	void render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip,
		SDL_Renderer* gRenderer,int w, int h);
	int getWidth();
	int getHigth();
	bool operator >(const GameObject &object)const;
	bool operator <(const GameObject &object)const;
	int getX();
	int getY();
};

#endif