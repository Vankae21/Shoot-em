#ifndef PLAYER_H
#define PLAYER_H

#include "inventory.h"
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
	Texture2D tex;
	bool is_taking_damage;
	Inventory* inventory;

	// VISUAL

	float sqz_time_elapsed;
	float height_dif;
	float height_dif_mltp;
	float height_dif_freq;

	// UI

	Color dmg_color;
} Player;

Player* init_player(Vector2 pos, Vector2 size, float speed, float max_hp, const char* tex_path);
void update_player(Player* player, Vector2 max_frame);
void draw_player(Player* player);
Rectangle get_player_rec(Player* player);
void add_to_inventory(Player* player, Weapon* weapon);
void remove_from_inventory(Player* player);
void change_slot(Player* player);
void change_weapon(Player* player, int index);

#endif