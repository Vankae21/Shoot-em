#ifndef WEAPON_H
#define WEAPON_H

#include "bullet.h"
#include "vutils.h"
#include <raylib.h>

typedef struct {
	enum {
		PISTOL,
		MINIGUN,
		SHOTGUN
	} type;

	Circle cir;
	bool is_picked_up;
	Vector2 dir;
	Bullet** bullets;
	unsigned int bullet_count;
	unsigned int max_ammo;
	int cur_ammo;
	bool is_reloading;
	float reload_time;
	float cur_reload_time;
	float damage;
	Texture2D bullet_tex;
	float shot_break;
	float shot_break_time_elapsed;
	bool can_shoot;
	float offset;
} Weapon;

Weapon* init_weapon(unsigned short type, Vector2 pos, const char* bullet_tex_path);
void update_weapon(Weapon* weapon);
void draw_weapon(Weapon* weapon, Texture2D weapon_spsheet);

#endif