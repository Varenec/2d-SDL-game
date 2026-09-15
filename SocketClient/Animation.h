#pragma once
#include "Structs.h"
#include <SDL3/SDL.h>

class Animation
{
private:
	struct Elem_anim;
	struct Var;
	Var* variables;
public:
	Animation(SDL_Renderer* renderer);
	void Add(const char* filepath, Coordinates coord, Texture_options opt, Duration dur = repeat);
	void Animate(float x, float y, Uint64 end_time, bool rotated = 0);
	void Restart();
	const int GetHeightTexture();
	const int GetWidthTexture();
	~Animation();
	Animation(const Animation&) = delete;
	Animation& operator=(const Animation&) = delete;
};