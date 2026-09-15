#pragma once
#include <array>
//
// == Textures and animations ==
//
struct Coordinates
{
	int x_beg;
	int y_beg;
	int x_end;
	int y_end;
};
struct Texture_options
{
	int frames_row;
	int frames_column;
	int delay_millsec = 100;
	float scale = 1;
};
struct Frame
{
	int x_beg;
	int y_beg;
	float w;
	float h;
	int current_x;
	int current_y;
};
enum Duration
{
	once,
	repeat,
};

//
// == Player ==
//
struct Player_options
{
	float x;
	float y;
	bool direction_x;
	float speed;
};
enum Move
{
	idle_horiz,
	idle_up,
	idle_down,
	idle_up_right,
	idle_up_left,
	idle_down_right,
	idle_down_left,
	walk_horiz,
	walk_up,
	walk_up_right,
	walk_up_left,
	walk_down,
	walk_down_right,
	walk_down_left,
	jump_horiz,
	jump_up,
	jump_up_right,
	jump_up_left,
	jump_down,
	jump_down_right,
	jump_down_left,
	shadow,
	Count,
};

//
// == Netwok ==
//
enum NetworkTransfer
{
	Scancode_D,
	Scancode_W,
	Scancode_A,
	Scancode_S,
	//Scancode_Space,
	CS_Count,
};
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
	std::array<bool, 4> PressButton;
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
	enum Type { spawn, movement, skinChange, disconnect } type;
};
struct Packet_skinchange
{
	Header_packet header;
	int skin_id;
};
struct Packet_movement
{
	Header_packet header;
	Player_state pl_state;
};