#include "socket.h"
#include <iostream>
class Server::Random_int
{
private:
	std::mt19937 gen;
	std::uniform_int_distribution<> skin_id_dis;
public:
	Random_int(int a, int b) : gen(std::random_device{}()), skin_id_dis(a, b) {}
	int Next() { return skin_id_dis(gen); }
};
struct Server::Player_information
{
	SOCKET pl_socket;
	Player_spawn_info pl_sp_info;
};
Server::Server(USHORT port) : max_id(0), rand_int(new Random_int(0, 2))
{
	InitializeNet(port);
}
void Server::Send_SI(Player_information* info)
{
	auto temp_pack = Packet_spawn{ Header_packet::spawn, info->pl_sp_info };
	for (int i = 0; i < players_info.size(); i++)
		if (players_info[i]->pl_sp_info.id != info->pl_sp_info.id)
			send(players_info[i]->pl_socket, (const char*)&temp_pack, sizeof(temp_pack), 0);
	for (int i = 0; i < players_info.size() - 1; i++)
	{
		temp_pack.sp_info = players_info[i]->pl_sp_info;
		send(info->pl_socket, (const char*)&temp_pack, sizeof(temp_pack), 0);
	}
}
void Server::Send_MI(Player_movement_info info)
{
	auto temp_pack = Packet_movement{ Header_packet::movement, info };
	for (int i = 0; i < players_info.size(); i++)
		if (players_info[i]->pl_sp_info.id != info.id)
			send(players_info[i]->pl_socket, (const char*)&temp_pack, sizeof(temp_pack), 0);
}
void Server::Send_SkinI(Player_skin_info info)
{
	auto temp_pack = Packet_skinchange{ Header_packet::skinChange, info };
	for (int i = 0; i < players_info.size(); i++)
		if (players_info[i]->pl_sp_info.id != info.id)
			send(players_info[i]->pl_socket, (const char*)&temp_pack, sizeof(temp_pack), 0);
}
void Server::Send_DiscI(int player_id)
{
	auto temp_pack = Packet_disconnect{ Header_packet::disconnect, player_id };
	for (int i = 0; i < players_info.size(); i++)
		if (players_info[i]->pl_sp_info.id != player_id)
			send(players_info[i]->pl_socket, (const char*)&temp_pack, sizeof(temp_pack), 0);
}
void Server::InitializeNet(USHORT port)
{
	WSADATA wsaDataa;
	WSAStartup(MAKEWORD(2, 2), &wsaDataa);
	SOCKET socket_server = socket(AF_INET, SOCK_STREAM, 0);
	this->server = socket_server;
	FD_ZERO(&read_fds);
	FD_SET(server, &read_fds);

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = port;
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	bind(socket_server, (sockaddr*)&serverAddr, sizeof(serverAddr));
	listen(socket_server, SOMAXCONN);
	std::cout << "listen...\n";
}
void Server::Read_fds()
{
	fd_set temp_fds = read_fds;
	select(0, &temp_fds, 0, 0, 0);
	if (FD_ISSET(server, &temp_fds))
	{
		SOCKET socket_client = accept(server, 0, 0);
		FD_SET(socket_client, &read_fds);
		players_info.push_back(new Player_information{ socket_client, { max_id++, rand_int->Next(), 50, 0, 2.0 } });
		send(socket_client, (const char*)&players_info.back()->pl_sp_info, sizeof(players_info.back()->pl_sp_info), 0);
		std::cout << "Connected player with id = " << players_info.back()->pl_sp_info.id << " and skin = " << players_info.back()->pl_sp_info.skin_id << std::endl;
		Send_SI(players_info.back());
	}
	for (int i = players_info.size() - 1; i >= 0; i--)
		if (FD_ISSET(players_info[i]->pl_socket, &temp_fds))
		{
			Header_packet header;
			int bytes = recv(players_info[i]->pl_socket, (char*)&header, sizeof(header), 0);
			if (bytes <= 0)
			{
				std::cout << "Disconnected player with id = " << players_info[i]->pl_sp_info.id << std::endl;
				Send_DiscI(players_info[i]->pl_sp_info.id);
				closesocket(players_info[i]->pl_socket);
				FD_CLR(players_info[i]->pl_socket, &read_fds);
				delete players_info[i];
				players_info.erase(players_info.begin() + i);
			}
			else
			{
				switch (header.type)
				{
				case Header_packet::movement:
				{
					Player_state state;
					recv(players_info[i]->pl_socket, (char*)&state, sizeof(state), 0);
					players_info[i]->pl_sp_info.x = state.x;
					players_info[i]->pl_sp_info.y = state.y;
					Send_MI(Player_movement_info{ players_info[i]->pl_sp_info.id, state.PressButton });
					break;
				}
				case Header_packet::skinChange:
				{
					int skin_id;
					recv(players_info[i]->pl_socket, (char*)&skin_id, sizeof(skin_id), 0);
					players_info[i]->pl_sp_info.skin_id = skin_id;
					std::cout << "Player with id " << players_info[i]->pl_sp_info.id << " changed skin to " << players_info[i]->pl_sp_info.skin_id << std::endl;
					Send_SkinI(Player_skin_info{players_info[i]->pl_sp_info.id, skin_id});
					break;
				}
				}
			}
		}
}
Server::~Server()
{
	delete rand_int;
	closesocket(server);
	for (Player_information* info : players_info)
	{
		closesocket(info->pl_socket);
		delete info;
	}
	WSACleanup();
}