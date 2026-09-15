#include "Texture.h"
#include <SDL3_image/SDL_image.h>
Texture::Texture(const char* file_path, SDL_Renderer* renderer, Coordinates coord, Texture_options opt, std::shared_ptr<SDL_Texture> ptrTexture) : renderer(renderer), opt(opt), now_time(0), frame({ 0, 0, 0, 0, 0, 0 }), /*file(file_path),*/ AnimateEnd(false)
{
	if (ptrTexture == nullptr)
		texture = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, file_path), SDL_DestroyTexture);
	else
		texture = ptrTexture;
	if (!texture.get()) SDL_Log("Failed to load texture: %s", SDL_GetError());
	frame.x_beg = coord.x_beg;
	frame.y_beg = coord.y_beg;
	frame.h = static_cast<float>((coord.y_end - coord.y_beg + 1) / opt.frames_column);
	frame.w = static_cast<float>((coord.x_end - coord.x_beg + 1) / opt.frames_row);
	SDL_SetTextureScaleMode(texture.get(), SDL_SCALEMODE_NEAREST);
}
void Texture::Animate(float x, float y, Uint64 end_time, bool rotated_x)
{
	if (end_time - now_time > 1.5 * opt.delay_millsec)
		now_time = SDL_GetTicks();
	if (end_time - now_time > opt.delay_millsec)
	{
		if (frame.current_x == opt.frames_row - 1 && frame.current_y == opt.frames_column - 1)
			AnimateEnd = true;
		else
		{
			if (frame.current_x == opt.frames_row - 1) frame.current_y++;
			++frame.current_x %= opt.frames_row;
		}
		now_time = SDL_GetTicks();
		//SDL_Log("%s  x = %d, y = %d", file, frame.current_x, frame.current_y);
	}
	SDL_FRect src = { frame.x_beg + frame.current_x * frame.w, frame.y_beg + frame.current_y * frame.h, frame.w, frame.h };
	SDL_FRect dst = { x, y, frame.w * opt.scale, frame.h * opt.scale };
	SDL_RenderTextureRotated(renderer, texture.get(), &src, &dst, 0, NULL, rotated_x ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}
const bool Texture::AnimationEnd() const
{
	return AnimateEnd;
}
void Texture::Restart()
{
	AnimateEnd = false;
	frame.current_x = 0;
	frame.current_y = 0;
}
std::shared_ptr<SDL_Texture> Texture::Get_ptrTexture()
{
	return texture;
}
const float Texture::GetHeight()
{
	return frame.h;
}
const float Texture::GetWidth()
{
	return frame.w;
}
