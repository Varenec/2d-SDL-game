#pragma once
#include <WS2tcpip.h>
#include "Background.h"
#include <vector>
#include <unordered_map>
class Network
{
private:
	struct PlayerController;
	std::vector<PlayerController*> players;
	Background* background;
	std::unordered_map<int, int> idToIndex;
	SDL_Renderer* renderer;
	SOCKET client;
	void HandlePacket();
public:
	Network(SDL_Renderer* renderer);
	int Initialize(SDL_Window* window, const bool* local_keyboard_state, USHORT port);
	void LocalPlayer_handleEvents(SDL_Event* event);
	void Update(Uint64 now_time);
	void closeNetwork() const;
	~Network();
	Network(const Network&) = delete;
	Network& operator=(const Network&) = delete;
};