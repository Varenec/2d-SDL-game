#include "Player.h"
#include "Skins.h"

struct Player::Struct_skin
{
	int skin_id;
	Skins* skin;
	Struct_skin(SDL_Renderer* renderer, int skin_id) : skin(new Skins(renderer)), skin_id(skin_id) {}
	~Struct_skin() { delete skin; }
	Struct_skin(const Struct_skin&) = delete;
	Struct_skin& operator=(const Struct_skin&) = delete;
};
Player::Player(SDL_Renderer* renderer, int skin_id, Player_options options) : renderer(renderer), skin_data(new Struct_skin(renderer, skin_id)), options(options) {}
Player::~Player() { delete skin_data; }
const float Player::GetX() { return options.x; }
const float Player::GetY() { return options.y; }
const float Player::GetWidthTexture() { return skin_data->skin->getAnimation(skin_data->skin_id, idle_horiz)->GetWidthTexture(); }
const float Player::GetHeightTexture() { return skin_data->skin->getAnimation(skin_data->skin_id, idle_horiz)->GetHeightTexture(); }
const int Player::GetSkin() { return skin_data->skin_id; }
void Player::SetSkin(int skin_id) { skin_data->skin_id = skin_id; }


LocalPlayer::LocalPlayer(SDL_Renderer* renderer, const bool* keyboard_state, int skin_id, Player_options options) : Player(renderer, skin_id, options), keyboard_state(keyboard_state)/*, background_x(background_x), background_y(background_y)*/ { }
std::array<bool, CS_Count> LocalPlayer::getPressedKeys()
{
	return std::array<bool, CS_Count>{keyboard_state[SDL_SCANCODE_D], keyboard_state[SDL_SCANCODE_W], keyboard_state[SDL_SCANCODE_A], keyboard_state[SDL_SCANCODE_S]};
}
int LocalPlayer::handleEvents(SDL_Event* event)
{
	int ret = 0;
	switch (event->type)
	{
	case SDL_EVENT_KEY_UP:
	{
		SDL_Scancode scan = event->key.scancode;
		switch (scan)
		{
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_A:
			skin_data->skin->getAnimation(skin_data->skin_id, walk_horiz)->Restart();
			break;
		case SDL_SCANCODE_W:
			skin_data->skin->getAnimation(skin_data->skin_id, skin_data->skin->getAnimation(skin_data->skin_id, walk_up) ? walk_up : walk_horiz)->Restart();
			break;
		case SDL_SCANCODE_S:
			skin_data->skin->getAnimation(skin_data->skin_id, skin_data->skin->getAnimation(skin_data->skin_id, walk_down) ? walk_down : walk_horiz)->Restart();
			break;
		}
		ret = 1;
		break;
	}
	case SDL_EVENT_KEY_DOWN:
	{
		SDL_Scancode scan = event->key.scancode;
		switch (scan)
		{
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_S:
			skin_data->skin->getAnimation(skin_data->skin_id, idle_horiz)->Restart();
			break;
		case SDL_SCANCODE_P:
			skin_data->skin_id = (skin_data->skin_id + 1) % SkinsCount;
			return 2;
			break;
		}
		ret = 1;
		break;
	}
	}
	return ret;
}
void LocalPlayer::Update(Uint64 now_time, Coordinates zone)
{
	Animation* anim = skin_data->skin->getAnimation(skin_data->skin_id, idle_horiz);
	if (keyboard_state[SDL_SCANCODE_D])
	{
		options.x = std::min(options.x + (keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDL_SCANCODE_S] ? options.speed / SDL_sqrtf(2) : options.speed), (float)zone.x_end);
		options.direction_x = true;
		anim = skin_data->skin->getAnimation(skin_data->skin_id, walk_horiz);
	}
	if (keyboard_state[SDL_SCANCODE_A])
	{
		options.x = std::max(options.x - (keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDL_SCANCODE_S] ? options.speed / SDL_sqrtf(2) : options.speed), (float)zone.x_beg);
		options.direction_x = false;
		anim = skin_data->skin->getAnimation(skin_data->skin_id, walk_horiz);
	}
	if (keyboard_state[SDL_SCANCODE_W])
	{
		options.y = std::max(options.y - (keyboard_state[SDL_SCANCODE_A] || keyboard_state[SDL_SCANCODE_D] ? options.speed / SDL_sqrtf(2) : options.speed), (float)zone.y_beg);
		anim = skin_data->skin->getAnimation(skin_data->skin_id, skin_data->skin->getAnimation(skin_data->skin_id, walk_up) ? walk_up : walk_horiz);
	}
	if (keyboard_state[SDL_SCANCODE_S])
	{
		options.y = std::min(options.y + (keyboard_state[SDL_SCANCODE_A] || keyboard_state[SDL_SCANCODE_D] ? options.speed / SDL_sqrtf(2) : options.speed), (float)zone.y_end);
		anim = skin_data->skin->getAnimation(skin_data->skin_id, skin_data->skin->getAnimation(skin_data->skin_id, walk_down) ? walk_down : walk_horiz);
	}
	anim->Animate(options.x, options.y, now_time, options.direction_x);
}



class NetworkPlayer::KeyboardAnalisys
{
private:
	const bool* keyboard_state;
	bool* previous_state;
	int key_count;
public:
	KeyboardAnalisys(const bool* keyboard_state, int key_count) : keyboard_state(keyboard_state), key_count(key_count), previous_state(new bool[key_count])
	{
		memcpy(previous_state, keyboard_state, key_count * sizeof(bool));
	}
	~KeyboardAnalisys() { delete[] previous_state; }
	void UpdateState() { memcpy(previous_state, keyboard_state, key_count * sizeof(bool)); }
	const bool IsKeyDown(int num) { return !previous_state[num] && keyboard_state[num]; }
	const bool IsKeyUp(int num) { return previous_state[num] && !keyboard_state[num]; }
};
class NetworkPlayer::ManageKeyboardState
{
private:
	bool* net_keyboard_state;
public:
	ManageKeyboardState() : net_keyboard_state(new bool[CS_Count]) {}
	void Update(bool buttons[CS_Count]) { memcpy(net_keyboard_state, buttons, CS_Count * sizeof(bool)); }
	const bool* GetKS() { return net_keyboard_state; }
	~ManageKeyboardState() { delete[] net_keyboard_state; }
};
NetworkPlayer::NetworkPlayer(SDL_Renderer* renderer, int skin_id, Player_options options) : Player(renderer, skin_id, options)
{
	manage_ks = new ManageKeyboardState();
	event = new KeyboardAnalisys(manage_ks->GetKS(), CS_Count);
}
NetworkPlayer::~NetworkPlayer()
{
	delete event;
	delete manage_ks;
}
void NetworkPlayer::Update_KS(std::array<bool, CS_Count> keys) { manage_ks->Update(keys.data()); }
void NetworkPlayer::handleEvents()
{
	event->UpdateState();
	if (event->IsKeyDown(Scancode_A) || event->IsKeyDown(Scancode_D) || event->IsKeyDown(Scancode_W) || event->IsKeyDown(Scancode_S))
		skin_data->skin->getAnimation(skin_data->skin_id, idle_horiz)->Restart();
	if (event->IsKeyUp(Scancode_A) || event->IsKeyUp(Scancode_D))
		skin_data->skin->getAnimation(skin_data->skin_id, walk_horiz)->Restart();
	if (event->IsKeyUp(Scancode_W))
		skin_data->skin->getAnimation(skin_data->skin_id, skin_data->skin->getAnimation(skin_data->skin_id, walk_up) ? walk_up : walk_horiz)->Restart();
	if (event->IsKeyUp(Scancode_S))
		skin_data->skin->getAnimation(skin_data->skin_id, skin_data->skin->getAnimation(skin_data->skin_id, walk_down) ? walk_down : walk_horiz)->Restart();
}
void NetworkPlayer::Update(Uint64 now_time, Coordinates zone)
{
	Animation* anim = skin_data->skin->getAnimation(skin_data->skin_id, idle_horiz);
	if (manage_ks->GetKS()[Scancode_D])
	{
		options.x = std::min(options.x + (manage_ks->GetKS()[Scancode_W] || manage_ks->GetKS()[Scancode_S] ? options.speed / SDL_sqrtf(2) : options.speed), (float)zone.x_end);
		options.direction_x = true;
		anim = skin_data->skin->getAnimation(skin_data->skin_id, walk_horiz);
	}
	if (manage_ks->GetKS()[Scancode_A])
	{
		options.x = std::max(options.x - (manage_ks->GetKS()[Scancode_W] || manage_ks->GetKS()[Scancode_S] ? options.speed / SDL_sqrtf(2) : options.speed), (float)zone.x_beg);
		options.direction_x = false;
		anim = skin_data->skin->getAnimation(skin_data->skin_id, walk_horiz);
	}
	if (manage_ks->GetKS()[Scancode_W])
	{
		options.y = std::max(options.y - (manage_ks->GetKS()[Scancode_A] || manage_ks->GetKS()[Scancode_D] ? options.speed / SDL_sqrtf(2) : options.speed), (float)zone.y_beg);
		anim = skin_data->skin->getAnimation(skin_data->skin_id, skin_data->skin->getAnimation(skin_data->skin_id, walk_up) ? walk_up : walk_horiz);
	}
	if (manage_ks->GetKS()[Scancode_S])
	{
		options.y = std::min(options.y + (manage_ks->GetKS()[Scancode_A] || manage_ks->GetKS()[Scancode_D] ? options.speed / SDL_sqrtf(2) : options.speed), (float)zone.y_end);
		anim = skin_data->skin->getAnimation(skin_data->skin_id, skin_data->skin->getAnimation(skin_data->skin_id, walk_down) ? walk_down : walk_horiz);
	}
	anim->Animate(options.x, options.y, now_time, options.direction_x);
}
