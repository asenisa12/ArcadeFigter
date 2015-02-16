#pragma once

#include "GameLabel.h"

class HealthBar : public GameLabel
{
	static const double xPercentige;
	static const double yPercentige;
	static const double wPercentige;
	static const double hPercentige;
	void renderHealth(SDL_Renderer *renderer, int health);
public:
	HealthBar(jsonObj data);
	void renderBar(SDL_Renderer *renderer,int health);
};