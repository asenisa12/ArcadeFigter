#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

class GameObject
{	
protected:
	int mWidth;
	int mHeight;
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
	int getHeight();
};

#endif