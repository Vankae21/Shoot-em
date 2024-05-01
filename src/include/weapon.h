#ifndef WEAPON_H
#define WEAPON_H

#include "bullet.h"
#include "vutils.h"
#include <raylib.h>

typedef struct
{
	Circle cir;
	bool is_picked_up;
	Vector2 dir;
	Bullet** bullets;
	unsigned int bullet_count;
	unsigned int max_ammo;
	unsigned int cur_ammo;
	bool is_reloading;
	float reload_time;
	float cur_reload_time;
	float damage;
	Texture2D tex;
	Texture2D bullet_tex;
} Weapon;

Weapon* init_weapon(Circle cir, unsigned int bullet_count, unsigned int max_ammo, float reload_time, float damage, const char* tex_path, const char* bullet_tex_path);
void update_weapon(Weapon* weapon);
void draw_weapon(Weapon* weapon);

#endif