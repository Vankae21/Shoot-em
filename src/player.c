#include "include/player.h"
#include "include/game.h"
#include "include/vutils.h"
#include "include/weapon.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

Player* init_player(Vector2 pos, Vector2 size, float speed, float max_hp, const char* tex_path)
{
	Player* player = calloc(1, sizeof(Player));

	player->pos = pos;
	player->size = size;
	player->speed = speed;
	player->move_dir = (Vector2){ 0 };
	player->cur_wpn = (void*)0;
	player->max_hp = max_hp;
	player->cur_hp = max_hp;
	player->is_facing_right = false;
	player->sprite_order = 1;
	player->tex = LoadTexture(tex_path);
	player->is_taking_damage = false;

	// VISUAL

	player->sqz_time_elapsed = 0;
	player->height_dif = 0;
	player->height_dif_mltp = 4;
	player->height_dif_freq = 3;

	// UI

	player->dmg_color = RED; player->dmg_color.a = 0;

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
		Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), *camera);
		Vector2 pointer = { .x = mousePos.x - player->pos.x - player->size.x/2, .y = mousePos.y - player->pos.y - player->size.y/2 };
		Vector2 pointer_dir = vec2_normalize(pointer);

		if(pointer_dir.x > 0)
		{
			player->is_facing_right = true;
		}
		else if(pointer_dir.x < 0)
		{
			player->is_facing_right = false;
		}

		player->cur_wpn->cir.center = (Vector2){ .x = player->pos.x + player->size.x/2 + pointer_dir.x * 64,
												.y = player->pos.y + player->size.y/2 + pointer_dir.y * 64};

		player->cur_wpn->dir = pointer_dir;

		update_weapon(player->cur_wpn);

		
		if(IsKeyPressed(KEY_Q))
		{
			player->cur_wpn->is_picked_up = false;
			player->cur_wpn->cur_reload_time = 0;
			player->cur_wpn->is_reloading = false;
			player->cur_wpn->dir = (Vector2){ 0 };
			player->cur_wpn = (void*)0;
		}
	}


	// TEST PURPOSE
	if(DEBUG)
	{
		if(IsMouseButtonPressed(1)) player->cur_hp = player->max_hp;
	}

	
	// UI
	short clr_mlpr = 2;
	if(player->is_taking_damage)
	{
		if(player->dmg_color.a < 127)
			player->dmg_color.a += clr_mlpr;
	}
	else
	{
		if(player->dmg_color.a > 0)
			player->dmg_color.a -= clr_mlpr;
		else
			player->dmg_color.a = 0;
	}

	

	player->height_dif = sinf(player->sqz_time_elapsed) * player->height_dif_mltp;
	player->sqz_time_elapsed += GetFrameTime() * player->height_dif_freq;

	if(player->sqz_time_elapsed > PI)
	{
		player->sqz_time_elapsed = 0;
	}

	if(vec2_len(player->move_dir) != 0)
	{
		if((int)player->height_dif == 0)
			player->sqz_time_elapsed = 0;
	}
}
void draw_player(Player* player)
{	

	Rectangle pl_rec = get_player_rec(player);

	pl_rec.height -= player->height_dif; pl_rec.y += player->height_dif;

	short tx_multiplier = player->is_facing_right ? -1 : 1;
	DrawTexturePro(player->tex, (Rectangle){ .x = 0, .y = 0, .width = player->tex.width * tx_multiplier, .height = player->tex.height },
					pl_rec, (Vector2){ 0 }, 0, WHITE);
}

Rectangle get_player_rec(Player* player)
{
	return (Rectangle){ .x = player->pos.x, .y = player->pos.y, .width = player->size.x, .height = player->size.y };
}