#include "include/enemy.h"
#include "include/game.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

Enemy* init_enemy(Vector2 pos, Vector2 size, float max_hp, float damage, float speed, const char* tex_path)
{
	Enemy* enemy = calloc(1, sizeof(Enemy));

	enemy->pos = pos;
	enemy->size = size;
	enemy->dir = (Vector2){ 0 };
	enemy->max_hp = max_hp;
	enemy->cur_hp = max_hp;
	enemy->damage = damage;
	enemy->speed = speed;
	enemy->is_dead = false;
	enemy->sprite_order = 0;
	enemy->is_facing_right = false;
	enemy->tex = LoadTexture(tex_path);

	return enemy;
}

void update_enemy(Enemy* enemy)
{
	if(enemy->dir.x > 0)
	{
		enemy->is_facing_right = true;
	}
	else if(enemy->dir.x < 0)
	{
		enemy->is_facing_right = false;
	}

	enemy->pos.x += enemy->dir.x * enemy->speed * GetFrameTime();
	enemy->pos.y += enemy->dir.y * enemy->speed * GetFrameTime();
}

void draw_enemy(Enemy* enemy)
{
	short tx_multiplier = enemy->is_facing_right ? -1 : 1;

	DrawTexturePro(enemy->tex, (Rectangle){ .x = 0, .y = 0, .width = enemy->tex.width * tx_multiplier, .height = enemy->tex.height },
					get_enemy_rec(enemy), (Vector2){ 0 }, 0, WHITE);

	if(enemy->cur_hp == enemy->max_hp) return;

	int bar_x = enemy->pos.x;
	int bar_y = enemy->pos.y - 16;
	int bar_width = enemy->size.x; int bar_height = 8;
	DrawRectangle(bar_x, bar_y, bar_width, bar_height, RED);
	DrawRectangle(bar_x, bar_y, bar_width * enemy->cur_hp / enemy->max_hp, bar_height, GREEN);
}

Rectangle get_enemy_rec(Enemy* enemy)
{
	return (Rectangle){ .x = enemy->pos.x, .y = enemy->pos.y, .width = enemy->size.x, .height = enemy->size.y };
}