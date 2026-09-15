#pragma once
#include "SDL3/SDL.h"
#include "Structs.h"
class Player
{
private:
	struct Struct_skin;
protected:
	Struct_skin* skin_data;
	SDL_Renderer* renderer;
	Player_options options;
	Player(SDL_Renderer* renderer, int skin_id, Player_options options);
public:
	const float GetX();
	const float GetY();
	const float GetHeightTexture();
	const float GetWidthTexture();
	const int GetSkin();
	void SetSkin(int skin_id);
	virtual void Update(Uint64 now_time, Coordinates zone) = 0;
	virtual ~Player();
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
};

class LocalPlayer : public Player
{
private:
	const bool* keyboard_state;
public:
	LocalPlayer(SDL_Renderer* renderer, const bool* keyboard_state, int skin_id, Player_options options);
	std::array<bool, CS_Count> getPressedKeys();
	void Update(Uint64 now_time, Coordinates zone) override;
	int handleEvents(SDL_Event* event);
};

class NetworkPlayer : public Player
{
private:
	class KeyboardAnalisys;
	class ManageKeyboardState;
	ManageKeyboardState* manage_ks;
	KeyboardAnalisys* event;
public:
	NetworkPlayer(SDL_Renderer* renderer, int skin_id, Player_options options);
	void Update(Uint64 now_time, Coordinates zone) override;
	void handleEvents();
	void Update_KS(std::array<bool, CS_Count> keys);
	~NetworkPlayer() override;
};