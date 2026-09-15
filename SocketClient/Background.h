#pragma once
#include "Player.h"
#include <vector>
class Background
{
private:
	class Random_int;
	struct AnimationCoord;
	struct Variables;
	struct PlayerInteraction;
	Random_int* rand_int;
	AnimationCoord* water_anim;
	AnimationCoord* grass_anim;
	Variables* vars;
	SDL_Renderer* renderer;
	std::vector<Player*> players;
	int randomGrass(int x, int y);
	void updWater(Uint64 now_time);
	void updBackgroundGrass(Uint64 now_time);
	void updForegroundGrass(float player_x, float player_y, float w, float h, Uint64 now_time);
	void updPlayer(Uint64 now_time);
public:
	Background(SDL_Renderer* renderer, SDL_Window* window);
	void Update(Uint64 now_time);
	void handleEvents(SDL_Event* event);
	void AddPlayer(Player* player);
	void DeletePlayer(int id);
	const int& getHeight();
	const int& getWidth();
	~Background();
	Background(const Background&) = delete;
	Background& operator=(const Background&) = delete;
};