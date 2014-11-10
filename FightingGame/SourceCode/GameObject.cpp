#include "GameObject.h"

bool GameObject::LoadFromFile(std::string path, SDL_Renderer* gRenderer)
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n",
			path.c_str(), IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0xF8));
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", 
				path.c_str(), SDL_GetError());
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	objTexture = newTexture;
	return objTexture != NULL;
}

void GameObject::free(){
	if (objTexture != NULL)
	{
		SDL_DestroyTexture(objTexture);
		objTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}
void GameObject::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(objTexture, alpha);
}

void GameObject::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip,
	SDL_Renderer* gRenderer,int w, int h)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, w, h };

	//Set clip rendering dimensions
	/*if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}*/

	//Render to screen
	SDL_RenderCopyEx(gRenderer, objTexture, clip, &renderQuad, angle, center, flip);
}

int GameObject::getWidth()
{
	return mWidth;
}

int GameObject::getHeight()
{
	return mHeight;
}
