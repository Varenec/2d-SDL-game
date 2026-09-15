#pragma once
#include <SDL3/SDL.h>
#include "Structs.h"
#include <memory>
class Texture
{
private:
	SDL_Renderer* renderer;
	std::shared_ptr<SDL_Texture> texture;
	Texture_options opt;
	Frame frame;
	Uint64 now_time;
	//const char* file;
	bool AnimateEnd;
public:
	Texture(const char* file_path, SDL_Renderer* renderer, Coordinates coord, Texture_options opt, std::shared_ptr<SDL_Texture> ptrTexture = nullptr);
	void Animate(float x, float y, Uint64 end_time, bool rotated = 0);
	const bool AnimationEnd() const;
	std::shared_ptr<SDL_Texture> Get_ptrTexture();
	const float GetHeight();
	const float GetWidth();
	void Restart();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
};
