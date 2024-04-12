#ifndef PLAYER_H
#define PLAYER_H

#include "weapon.h"
#include <raylib.h>

typedef struct
{
	Vector2 pos;
	Vector2 size;
	Vector2 move_dir;
	float speed;
	Weapon* cur_wpn;
	float max_hp;
	float cur_hp;
	bool is_facing_right;
	int sprite_order;
} Player;

Player* init_player(Vector2 pos, Vector2 size, float speed, float max_hp);
void update_player(Player* player);
void draw_player(Player* player);
Rectangle get_player_rec(Player* player);

#endif