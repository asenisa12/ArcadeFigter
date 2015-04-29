#pragma once

#include "GameLabel.h"

class HealthBar : public GameLabel
{
	friend class GameLabel;

	static const double xPercentige;
	static const double yPercentige;
	static const double wPercentige;
	static const double hPercentige;
	int maxHealth;
	void renderHealth(SDL_Renderer *renderer, int health, int y);
public:
	HealthBar(Value& data, int scrW, int scrH);
	void renderBar(SDL_Renderer *renderer, int health, int y);
	void setMaxHealth(int health);
};