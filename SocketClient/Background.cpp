#include "Background.h"
#include "Texture.h"
#include "Animation.h"
#include <random>
#include <unordered_set>
struct ConstVariables
{
	static  constexpr float scale = 5.0f;
	static constexpr int common_height = 1080, common_width = 1920;
	static constexpr int TextureinPixels = (int)(scale * 16);
	static constexpr int TexBackwidth = 40;
	static constexpr int TexBackheight = 23;
};
class Background::Random_int
{
private:
	std::mt19937 gen;
	std::uniform_int_distribution<> skin_id_dis;
public:
	Random_int(int a, int b) : gen(std::random_device{}()), skin_id_dis(a, b) {}
	int Next() { return skin_id_dis(gen); }
};
struct Coord { int x_in, y_in; };
struct Background::AnimationCoord
{
	Animation* anim;
	std::array<std::array<std::vector<Coord>, ConstVariables::TexBackwidth>, ConstVariables::TexBackheight> coords;
	std::vector<Coord> fullcoords;
	~AnimationCoord() { delete anim; }
};
struct Background::Variables
{
	SDL_Texture* background_tex;
	SDL_Texture* water_tex;
	SDL_Texture* grass_tex;
	SDL_Texture* players_tex;
	int height, width;
	~Variables()
	{
		SDL_DestroyTexture(background_tex);
		SDL_DestroyTexture(water_tex);
		SDL_DestroyTexture(grass_tex);
		SDL_DestroyTexture(players_tex);
	}
};
enum Type
{
	angle_up_right,
	angle_up_left,
	angle_down_left,
	angle_down_right,
	up,
	down,
	right,
	left,
	grass,
	grass_1,
	grass_2,
	grass_3,
	grass_4,
	grass_5,
	grass_6,
	grass_7,
	grass_8,
	fence_left_up,
	fence_vert,
	fence_vert_end,
	fence_up,
	fence_right_up,
	Count_type,
};
Background::Background(SDL_Renderer* renderer, SDL_Window* window) : renderer(renderer), vars(new Variables{}), rand_int(new Random_int(1, 32))
{
	grass_anim = new AnimationCoord{ new Animation(renderer), {} };
	water_anim = new AnimationCoord{ new Animation(renderer), {} };
	std::array<Texture*, Count_type> tex_types{};
	Texture_options opt = { 1, 1, 1, ConstVariables::scale };
	tex_types[angle_up_left] = new Texture("Sprout_Lands\\Tilesets\\Hills.png", renderer, { 0, 0, 15, 15 }, opt);
	tex_types[angle_up_right] = new Texture("", renderer, { 32, 0, 47, 15 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[angle_down_left] = new Texture("", renderer, { 0, 32, 15, 47 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[angle_down_right] = new Texture("", renderer, { 32, 32, 47, 47 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[up] = new Texture("", renderer, { 16, 0, 31, 15 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[right] = new Texture("", renderer, { 32, 16, 47, 31 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[left] = new Texture("", renderer, { 0, 16, 15, 31 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[down] = new Texture("", renderer, { 16, 32, 31, 47 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[grass] = new Texture("", renderer, { 16, 16, 31, 31 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[grass_1] = new Texture("", renderer, { 0, 90, 15, 105 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[grass_2] = new Texture("", renderer, { 16, 90, 31, 105 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[grass_3] = new Texture("", renderer, { 32, 90, 47, 105 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[grass_4] = new Texture("", renderer, { 48, 90, 63, 105 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[grass_5] = new Texture("", renderer, { 64, 90, 79, 105 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[grass_6] = new Texture("", renderer, { 80, 90, 95, 105 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[grass_7] = new Texture("", renderer, { 96, 90, 111, 105 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[grass_8] = new Texture("", renderer, { 112, 90, 127, 105 }, opt, tex_types[angle_up_left]->Get_ptrTexture());
	tex_types[fence_left_up] = new Texture("Sprout_Lands\\Tilesets\\Fences.png", renderer, { 16, 0, 31, 15 }, opt);
	tex_types[fence_vert] = new Texture("", renderer, { 0, 16, 15, 31 }, opt, tex_types[fence_left_up]->Get_ptrTexture());
	tex_types[fence_vert_end] = new Texture("", renderer, { 0, 32, 15, 47 }, opt, tex_types[fence_left_up]->Get_ptrTexture());
	tex_types[fence_right_up] = new Texture("", renderer, { 48, 0, 63, 15 }, opt, tex_types[fence_left_up]->Get_ptrTexture());
	tex_types[fence_up] = new Texture("", renderer, { 32, 48, 47, 63 }, opt, tex_types[fence_left_up]->Get_ptrTexture());

	water_anim->anim->Add("Sprout_Lands\\Tilesets\\Water.png", { 0, 0, 63, 15 }, { 4, 1, 800, ConstVariables::scale });
	grass_anim->anim->Add("Sprout_Lands\\Tilesets\\Grass_blade.png", { 0, 0, 63, 15 }, { 4, 1, 1000, ConstVariables::scale });

	SDL_GetWindowSizeInPixels(window, &vars->width, &vars->height);
	vars->background_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ConstVariables::TextureinPixels * ConstVariables::TexBackwidth, ConstVariables::TextureinPixels * ConstVariables::TexBackheight);
	vars->water_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ConstVariables::TextureinPixels * ConstVariables::TexBackwidth, ConstVariables::TextureinPixels * ConstVariables::TexBackheight);
	vars->grass_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ConstVariables::TextureinPixels * ConstVariables::TexBackwidth, ConstVariables::TextureinPixels * ConstVariables::TexBackheight);
	vars->players_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ConstVariables::TextureinPixels * ConstVariables::TexBackwidth, ConstVariables::TextureinPixels * ConstVariables::TexBackheight);


	SDL_SetTextureScaleMode(vars->background_tex, SDL_SCALEMODE_NEAREST);
	SDL_SetTextureScaleMode(vars->grass_tex, SDL_SCALEMODE_NEAREST);
	SDL_SetTextureScaleMode(vars->water_tex, SDL_SCALEMODE_NEAREST);
	SDL_SetTextureScaleMode(vars->players_tex, SDL_SCALEMODE_NEAREST);

	SDL_SetRenderTarget(renderer, vars->background_tex);
	for (int i = 0; i < ConstVariables::TexBackwidth; i++)
	{
		tex_types[i == 0 ? angle_up_left : i == ConstVariables::TexBackwidth - 1 ? angle_up_right : up]->Animate((float)(ConstVariables::TextureinPixels * i), 0.0f, 0);
		tex_types[i == 0 ? fence_left_up : i == ConstVariables::TexBackwidth - 1 ? fence_right_up : fence_up]->Animate((float)(ConstVariables::TextureinPixels * i), 0.0f, 0);
	}
	for (int i = 1; i < ConstVariables::TexBackheight - 2; i++)
		for (int j = 0; j < ConstVariables::TexBackwidth; j++)
		{
			tex_types[j == 0 ? left : j == ConstVariables::TexBackwidth - 1 ? right : randomGrass(j, i)]->Animate((float)(j * ConstVariables::TextureinPixels), (float)(i * ConstVariables::TextureinPixels), 0);
			if (j == 0 || j == ConstVariables::TexBackwidth - 1)
				tex_types[i == ConstVariables::TexBackheight - 3 ? fence_vert_end : fence_vert]->Animate(j * ConstVariables::TextureinPixels, i * ConstVariables::TextureinPixels, 0);
		}
	for (int i = 0; i < ConstVariables::TexBackwidth; i++)
		tex_types[i == 0 ? angle_down_left : i == ConstVariables::TexBackwidth - 1 ? angle_down_right : down]->Animate(i * ConstVariables::TextureinPixels, (ConstVariables::TexBackheight - 2) * ConstVariables::TextureinPixels, 0);
	SDL_SetRenderTarget(renderer, NULL);

	for (int i = 0; i < ConstVariables::TexBackheight; i++)
		for (int j = 0; j < ConstVariables::TexBackwidth; j++)
			if (i == 0 || j == 0 || i == ConstVariables::TexBackheight - 1 || j == ConstVariables::TexBackwidth - 1)
				water_anim->fullcoords.push_back({ j * ConstVariables::TextureinPixels, i * ConstVariables::TextureinPixels });

	for (auto tex : tex_types)
		delete tex;
}
int Background::randomGrass(int x, int y)
{
	switch (rand_int->Next())
	{
	case 1:
		return grass_1;
		break;
	case 2:
		return grass_2;
		break;
	case 3:
		return grass_3;
		break;
	case 4:
		return grass_4;
		break;
	case 5:
		return grass_5;
		break;
	case 6:
		return grass_6;
		break;
	case 7:
		return grass_7;
		break;
	case 8:
		return grass_8;
		break;
	default:
		int x_in = ConstVariables::TextureinPixels / 2;
		int y_in = rand_int->Next();
		grass_anim->coords[y][x].push_back(Coord{ 0, 0 });
		grass_anim->coords[y][x].push_back(Coord{ x_in, y_in });
		grass_anim->fullcoords.push_back({ x * ConstVariables::TextureinPixels, y * ConstVariables::TextureinPixels });
		grass_anim->fullcoords.push_back({ x * ConstVariables::TextureinPixels + x_in, y * ConstVariables::TextureinPixels + y_in });
		return grass;
		break;
	}
}
void Background::AddPlayer(Player* player) { this->players.push_back(player); }
void Background::DeletePlayer(int id) { players[id] = nullptr; }
void Background::updWater(Uint64 now_time)
{
	SDL_SetRenderTarget(renderer, vars->water_tex);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	for (auto& cord : water_anim->fullcoords)
		water_anim->anim->Animate(cord.x_in, cord.y_in, now_time);
	SDL_SetRenderTarget(renderer, NULL);
}
void Background::updBackgroundGrass(Uint64 now_time)
{
	SDL_SetRenderTarget(renderer, vars->grass_tex);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	for (auto& cord : grass_anim->fullcoords)
		grass_anim->anim->Animate(cord.x_in, cord.y_in, now_time);
}
void Background::updForegroundGrass(float player_x_pol, float player_y_pol, float w, float h, Uint64 now_time)
{
	int chunk_x = std::floor(player_x_pol / ConstVariables::TextureinPixels);
	int chunk_y = std::floor((player_y_pol + h) / ConstVariables::TextureinPixels) + 1;
	int end_chunk_x = std::floor((player_x_pol + w) / ConstVariables::TextureinPixels);
	for (int i = chunk_x; i <= end_chunk_x; i++)
		for (auto& cord : grass_anim->coords[chunk_y][i])
			grass_anim->anim->Animate(i * ConstVariables::TextureinPixels + cord.x_in, chunk_y * ConstVariables::TextureinPixels + cord.y_in, now_time);
}

void Background::updPlayer(Uint64 now_time)
{
	std::vector<Player*> sorted_players;
	for (Player* p : players) 
		if (p != nullptr) 
			sorted_players.push_back(p);
	std::sort(sorted_players.begin(), sorted_players.end(),
		[](Player* a, Player* b)
		{ if (a == nullptr || b == nullptr) return false; return a->GetY() < b->GetY(); });
	SDL_SetRenderTarget(renderer, vars->players_tex);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	for (auto player : sorted_players)
	{
		player->Update(now_time, { int((float)ConstVariables::TextureinPixels-0.5f*player->GetWidthTexture()), int(-0.5f*player->GetHeightTexture()), (ConstVariables::TexBackwidth - 2) * ConstVariables::TextureinPixels, (ConstVariables::TexBackheight - 2) * ConstVariables::TextureinPixels - (int)(player->GetHeightTexture()*2.0f)});
		updForegroundGrass(player->GetX(), player->GetY(), player->GetWidthTexture(), player->GetHeightTexture(), now_time);
	}
	SDL_SetRenderTarget(renderer, NULL);
}
void Background::Update(Uint64 now_time)
{
	updWater(now_time);
	updBackgroundGrass(now_time);
	updPlayer(now_time);
	float x_temp = std::max(0.0f, std::min(players[0]->GetX() + players[0]->GetWidthTexture() / 2 - vars->width / 2 + ConstVariables::TextureinPixels / 2, (float)(ConstVariables::TextureinPixels * ConstVariables::TexBackwidth) - (float)vars->width));
	float y_temp = std::max(0.0f, std::min(players[0]->GetY() + players[0]->GetHeightTexture() / 2 - vars->height / 2 + ConstVariables::TextureinPixels / 2, (float)(ConstVariables::TextureinPixels * ConstVariables::TexBackheight) - (float)vars->height));
	SDL_FRect src = { x_temp, y_temp, vars->width, vars->height };
	SDL_FRect dst = { 0, 0, vars->width, vars->height };
	SDL_RenderTexture(renderer, vars->water_tex, &src, &dst);
	SDL_RenderTexture(renderer, vars->background_tex, &src, &dst);
	SDL_RenderTexture(renderer, vars->grass_tex, &src, &dst);
	SDL_RenderTexture(renderer, vars->players_tex, &src, &dst);
}
void Background::handleEvents(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_EVENT_WINDOW_RESIZED:
		vars->width = event->window.data1;
		vars->height = event->window.data2;
		break;
	}
}
const int& Background::getHeight()
{
	return vars->height;
}
const int& Background::getWidth()
{
	return vars->width;
}
Background::~Background()
{
	delete grass_anim;
	delete water_anim;
	delete rand_int;
	delete vars;
}