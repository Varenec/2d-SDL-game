#include "Animation.h"
#include "Texture.h"
#include <vector>
struct Animation::Elem_anim
{
	Texture* texture;
	const char* filepath;
	Duration dur;
	~Elem_anim() { delete texture; }
	Elem_anim(const Elem_anim&) = delete;
	Elem_anim& operator=(const Elem_anim&) = delete;
};
struct Animation::Var
{
	SDL_Renderer* renderer;
	std::vector<Elem_anim*> textures;
	int current_texture;
	Var(SDL_Renderer* renderer) : renderer(renderer), current_texture(0) {}
	~Var()
	{
		for (auto anim : textures)
			delete anim;
	}
	Var(const Var&) = delete;
	Var& operator=(const Var&) = delete;
};
Animation::Animation(SDL_Renderer* renderer) : variables(new Var(renderer)) {}
void Animation::Add(const char* filepath, Coordinates coord, Texture_options opt, Duration dur)
{
	std::shared_ptr<SDL_Texture> temp = nullptr;
	for (const Elem_anim* tex : variables->textures)
		if (tex->filepath == 0)
		{
			temp = tex->texture->Get_ptrTexture();
			break;
		}
	variables->textures.push_back(new Elem_anim{ new Texture(filepath, variables->renderer, coord, opt, temp), filepath, dur });
}
void Animation::Animate(float x, float y, Uint64 end_time, bool rotated_x)
{
	int& cur_tex = variables->current_texture;
	size_t size = variables->textures.size();

	while (variables->textures[cur_tex]->texture->AnimationEnd())
		++cur_tex %= size;

	Texture* texture = variables->textures[cur_tex]->texture;
	Duration dur = variables->textures[cur_tex]->dur;

	texture->Animate(x, y, end_time, rotated_x);
	if (texture->AnimationEnd())
	{
		if (dur == repeat)
			texture->Restart();
		++cur_tex %= size;
	}
}
void Animation::Restart()
{
	variables->current_texture = 0;
	for (size_t i = 0; i < variables->textures.size(); i++)
		variables->textures[i]->texture->Restart();
}
const int Animation::GetHeightTexture()
{
	return variables->textures[variables->current_texture]->texture->GetHeight();
}
const int Animation::GetWidthTexture()
{
	return variables->textures[variables->current_texture]->texture->GetWidth();
}
Animation::~Animation()
{
	delete variables;
}