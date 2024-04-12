#include "include/player.h"
#include "include/game.h"
#include "include/utils.h"
#include "include/weapon.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

Player* init_player(Vector2 pos, Vector2 size, float speed, float max_hp)
{
	Player* player = calloc(1, sizeof(Player));

	player->pos = pos;
	player->size = size;
	player->speed = speed;
	player->move_dir = (Vector2){ 0 };
	player->cur_wpn = (void*)0;
	player->max_hp = max_hp;
	player->cur_hp = max_hp;
	player->is_facing_right = true;
	player->sprite_order = 1;
	
	return player;
}

void update_player(Player* player)
{
	Vector2 input = { .x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A), .y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W) };

	if(input.x > 0)
	{
		player->is_facing_right = true;
	}
	else if(input.x < 0)
	{
		player->is_facing_right = false;
	}

	player->move_dir = vec2_normalize(input);

	player->pos.x += player->move_dir.x * player->speed * GetFrameTime();
	player->pos.y += player->move_dir.y * player->speed * GetFrameTime();

	if(player->cur_wpn)
	{
		Vector2 pointer = { .x = GetMouseX() - player->pos.x - player->size.x/2, .y = GetMouseY() - player->pos.y - player->size.y/2 };
		Vector2 pointer_dir = vec2_normalize(pointer);

		player->cur_wpn->cir.center = (Vector2){ .x = player->pos.x + player->size.x/2 + pointer_dir.x * 64,
												.y = player->pos.y + player->size.y/2 + pointer_dir.y * 64};

		player->cur_wpn->dir = pointer_dir;

		update_weapon(player->cur_wpn);

		
		if(IsKeyPressed(KEY_Q))
		{
			player->cur_wpn->is_picked_up = false;
			player->cur_wpn->cur_reload_time = 0;
			player->cur_wpn->is_reloading = false;
			player->cur_wpn = (void*)0;
		}
	}

	if(IsMouseButtonPressed(1)) player->cur_hp = player->max_hp;
}

void draw_player(Player* player)
{
	DrawRectangleV(player->pos, player->size, BLACK);
}

Rectangle get_player_rec(Player* player)
{
	return (Rectangle){ .x = player->pos.x, .y = player->pos.y, .width = player->size.x, .height = player->size.y };
}