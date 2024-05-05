#include "include/enemy.h"
#include "include/game.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

Enemy* init_enemy(unsigned short type, Vector2 pos, Vector2 size, float max_hp, float damage, float speed)
{
	Enemy* enemy = calloc(1, sizeof(Enemy));

	enemy->type = type;
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
	enemy->knockback_complete = true;
	enemy->knockback_pos = enemy->pos;

	return enemy;
}

void update_enemy(Enemy* enemy)
{
	if(!enemy->knockback_complete)
	{
		enemy->pos = vec2_translate(enemy->pos, enemy->knockback_pos, 400);
		if(vec2_cmp(enemy->pos, enemy->knockback_pos))
		{
			enemy->knockback_complete = true;
		}
		else
		{
			return;
		}
	}
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

void draw_enemy(Enemy* enemy, Texture2D enemies_spsheet)
{
	short tx_multiplier = enemy->is_facing_right ? -1 : 1;

	Color color = enemy->knockback_complete ? WHITE : RED;

	DrawTexturePro(enemies_spsheet, (Rectangle){ .x = 32 * enemy->type, .y = 0, .width = 32 * tx_multiplier, .height = 32 },
					get_enemy_rec(enemy), (Vector2){ 0 }, 0, color);

	if(enemy->cur_hp == enemy->max_hp) return;

	int bar_x = enemy->pos.x;
	int bar_y = enemy->pos.y - 16;
	int bar_width = enemy->size.x; int bar_height = 8;
	DrawRectangle(bar_x, bar_y, bar_width, bar_height, RED);
	DrawRectangle(bar_x, bar_y, bar_width * enemy->cur_hp / enemy->max_hp, bar_height, DARKGREEN);
}

void draw_enemy_shadow(Enemy* enemy)
{
	Rectangle e_rec = get_enemy_rec(enemy);

	DrawRectangle(e_rec.x + SHADOW_OFFSET, e_rec.y + SHADOW_OFFSET, e_rec.width, e_rec.height, SHADOW_COLOR);
}

Rectangle get_enemy_rec(Enemy* enemy)
{
	return (Rectangle){ .x = enemy->pos.x, .y = enemy->pos.y, .width = enemy->size.x, .height = enemy->size.y };
}