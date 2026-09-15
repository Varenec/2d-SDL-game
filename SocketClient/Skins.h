#pragma once
#include "Animation.h"
static constexpr int SkinsCount = 6;
class Skins
{
private:
	SDL_Renderer* render;
	Animation* skins[SkinsCount][Move::Count];
public:
	Skins(SDL_Renderer* render);
	~Skins();
	Skins(const Skins&) = delete;
	Skins& operator=(const Skins&) = delete;
	Animation* getAnimation(int num_skin, Move move) const;
};
