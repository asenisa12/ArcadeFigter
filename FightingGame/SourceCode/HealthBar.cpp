#include "HealthBar.h"

const double HealthBar::xPercentige = 0.204724;
const double HealthBar::yPercentige = 0.6538;
const double HealthBar::wPercentige = 0.78906;
const double HealthBar::hPercentige = 0.2692307;

HealthBar::HealthBar(jsonObj data, int scrW, int scrH)
	:GameLabel(data, scrW, scrH)
{}

void HealthBar::renderHealth(SDL_Renderer *renderer, int health)
{
	if (health == NULL) health = 0;
	double healthBarW = width_*wPercentige * ((double)health / (double)maxHealth);
	SDL_Rect healthbar = { width_*xPercentige, height_*yPercentige, healthBarW, height_*hPercentige };
	SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0);
	SDL_RenderFillRect(renderer, &healthbar);
	SDL_RenderDrawRect(renderer, &healthbar);
}

void HealthBar::renderBar(SDL_Renderer *renderer, int health, int y)
{
	renderLabel(0, y, renderer);
	renderHealth(renderer, health);
}

void HealthBar::setMaxHealth(int health)
{
	maxHealth = health;
}