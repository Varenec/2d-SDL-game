#include "Network.h"
#include <memory>

struct Network::PlayerController
{
	int id;
	Player* player;
	PlayerController(int id, Player* player) : id(id), player(player) {}
	~PlayerController() { delete player; }
	PlayerController(const PlayerController&) = delete;
	PlayerController& operator=(const PlayerController&) = delete;
};
Network::Network(SDL_Renderer* renderer) : renderer(renderer), client(INVALID_SOCKET), background(nullptr) {}
int Network::Initialize(SDL_Window* window, const bool* local_keyboard_state, USHORT port)
{
	WSADATA wsa;
	sockaddr_in serveraddr;
	u_long mode = 1;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	client = socket(AF_INET, SOCK_STREAM, 0);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = port;
	inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);
	if (connect(client, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
		return 2;

	background = new Background(renderer, window);
	Player_spawn_info temp;
	recv(client, (char*)&temp, sizeof(temp), 0);
	players.push_back(new PlayerController(temp.id, new LocalPlayer(renderer, local_keyboard_state, temp.skin_id, { temp.x, temp.y, true, temp.speed })));
	background->AddPlayer(players.back()->player);
	ioctlsocket(client, FIONBIO, &mode);
	return 0;
}
void Network::LocalPlayer_handleEvents(SDL_Event* event)
{
	int ret = static_cast<LocalPlayer*>(players[0]->player)->handleEvents(event);
	if (ret == 1)
	{
		auto state = Packet_movement{ Header_packet::movement ,Player_state{ players[0]->player->GetX(), players[0]->player->GetY(), static_cast<LocalPlayer*>(players[0]->player)->getPressedKeys() } };
		send(client, (const char*)&state, sizeof(state), 0);
	}
	if (ret == 2)
	{
		auto skin = Packet_skinchange{ Header_packet::skinChange, players[0]->player->GetSkin() };
		send(client, (const char*)&skin, sizeof(skin), 0);
	}
	background->handleEvents(event);
}
void Network::Update(Uint64 now_time)
{
	for (int i = 1; i < players.size(); i++)
		if (players[i]->player != nullptr)
		static_cast<NetworkPlayer*>(players[i]->player)->handleEvents();
	HandlePacket();
	background->Update(now_time);
}
void Network::HandlePacket()
{
	Header_packet header;
	while (recv(client, (char*)&header, sizeof(header), 0) > 0)
		switch (header.type)
		{
		case Header_packet::spawn:
		{
			Player_spawn_info sp_info;
			recv(client, (char*)&sp_info, sizeof(sp_info), 0);
			players.push_back(new PlayerController(sp_info.id, new NetworkPlayer(renderer, sp_info.skin_id, { sp_info.x, sp_info.y, true, sp_info.speed })));
			background->AddPlayer(players.back()->player);
			idToIndex[sp_info.id] = (int)players.size() - 1;
			break;
		}
		case Header_packet::movement:
		{
			Player_movement_info mp_info;
			recv(client, (char*)&mp_info, sizeof(mp_info), 0);
			static_cast<NetworkPlayer*>(players[idToIndex[mp_info.id]]->player)->Update_KS(mp_info.PressButton);
			break;
		}
		case Header_packet::skinChange:
		{
			Player_skin_info sk_info;
			recv(client, (char*)&sk_info, sizeof(sk_info), 0);
			players[idToIndex[sk_info.id]]->player->SetSkin(sk_info.skin_id);
			break;
		}
		case Header_packet::disconnect:
		{
			int player_id;
			recv(client, (char*)&player_id, sizeof(player_id), 0);
			delete players[idToIndex[player_id]]->player;
			players[idToIndex[player_id]]->player = nullptr;
			background->DeletePlayer(idToIndex[player_id]);
		}
		}
}
void Network::closeNetwork() const
{
	closesocket(client);
	WSACleanup();
}
Network::~Network()
{
	delete background;
	for (PlayerController* pl_co : players)
		delete pl_co;
}
