#pragma once
#include <vector>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "Structs.h"
#include <random>
class Server
{
private:
	class Random_int;
	struct Player_information;
	Random_int* rand_int;
	std::vector<Player_information*> players_info;
	SOCKET server;
	fd_set read_fds;
	int max_id;
	void InitializeNet(USHORT port);
	void Send_SI(Player_information* info);
	void Send_MI(Player_movement_info info);
	void Send_SkinI(Player_skin_info info);
	void Send_DiscI(int player_id);
public:
	Server(USHORT port);
	void Read_fds();
	~Server();
};