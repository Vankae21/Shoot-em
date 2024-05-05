#ifndef ENEMY_H
#define ENEMY_H

#include "vutils.h"
#include <raylib.h>

#define ENEMY_MAX_COIN_DROP 3
#define ENEMY_MEDKIT_DROP_CHANCE 0.05f

typedef struct
{
	enum
	{
		ZOMBIE
	} type;

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
	bool knockback_complete;
	Vector2 knockback_pos;
} Enemy;

Enemy* init_enemy(unsigned short type, Vector2 pos, Vector2 size, float max_hp, float damage, float speed);
void update_enemy(Enemy* enemy);
void draw_enemy(Enemy* enemy, Texture2D enemies_spsheet);
Rectangle get_enemy_rec(Enemy* enemy);

#endif