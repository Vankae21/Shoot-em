#include "include/enemy.h"
#include "include/game.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

Enemy* init_enemy(Vector2 pos, Vector2 size, float max_hp, float damage, float speed)
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

	return enemy;
}

void update_enemy(Enemy* enemy)
{
	enemy->pos.x += enemy->dir.x * enemy->speed * GetFrameTime();
	enemy->pos.y += enemy->dir.y * enemy->speed * GetFrameTime();
}

void draw_enemy(Enemy* enemy)
{
	Color e_color = enemy->sprite_order == 0 ? RED : YELLOW;
	DrawRectangleV(enemy->pos, enemy->size, e_color);

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