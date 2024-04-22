#ifndef ENEMY_H
#define ENEMY_H

#include "utils.h"
#include <raylib.h>

typedef struct
{
	Vector2 pos;
	Vector2 size;
	Vector2 dir;
	float max_hp;
	float cur_hp;
	float damage;
	float speed;
	bool is_dead;
	int sprite_order;
	bool is_facing_right;
	Texture2D tex;
} Enemy;

Enemy* init_enemy(Vector2 pos, Vector2 size, float max_hp, float damage, float speed, const char* tex_path);
void update_enemy(Enemy* enemy);
void draw_enemy(Enemy* enemy);
Rectangle get_enemy_rec(Enemy* enemy);

#endif