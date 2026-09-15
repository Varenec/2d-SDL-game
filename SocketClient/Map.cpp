//#include "Player.h"
//class LocalPlayer : public Player
//{
//private:
//	const bool* keyboard_state;
//public:
//	LocalPlayer(SDL_Renderer* renderer, const bool* keyboard_state, int skin_id, Player_options options) : Player(renderer, skin_id, options), keyboard_state(keyboard_state) { }
//	std::array<bool, CS_Count> getPressedKeys();
//	void Update(Uint64 now_time) override;
//	int handleEvents(SDL_Event* event);
//};