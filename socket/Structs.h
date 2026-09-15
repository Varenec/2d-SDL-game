#pragma once
#include <array>
struct Player_spawn_info
{
	int id;
	int skin_id;
	float x;
	float y;
	float speed;
};
struct Player_movement_info
{
	int id;
	std::array<bool, 4>PressButtons;
};
struct Player_skin_info
{
	int id;
	int skin_id;
};
struct Player_state
{
	float x;
	float y;
	std::array<bool, 4> PressButton;
};
struct Header_packet
{
	enum Type {spawn, movement, skinChange, disconnect} type;
};
struct Packet_spawn
{
	Header_packet header;
	Player_spawn_info sp_info;
};
struct Packet_movement
{
	Header_packet header;
	Player_movement_info mp_info;
};
struct Packet_skinchange
{
	Header_packet header;
	Player_skin_info sk_info;
};
struct Packet_disconnect
{
	Header_packet header;
	int player_id;
};
enum NetworkTransfer
{
	Scancode_D,
	Scancode_W,
	Scancode_A,
	Scancode_S,
	//Scancode_Space,
	CS_Count,
};