#include "include/player.h"
#include "include/game.h"
#include "include/inventory.h"
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
	player->inventory = init_inventory(/*SLOT COUNT--->*/ 5);

	// VISUAL

	player->sqz_time_elapsed = 0;
	player->height_dif = 0;
	player->height_dif_mltp = 4;
	player->height_dif_freq = 3;

	// UI

	player->dmg_color = RED; player->dmg_color.a = 0;

	return player;
}

void update_player(Player* player, Vector2 max_frame)
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

	float move_speed_divisor = 1;

	// CHANGE INVENTORY SLOT //----------
	change_slot(player);

	if (player->cur_wpn) {
		Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), *(gamecamera->camera));
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

		// WEAPON

		player->cur_wpn->dir = pointer_dir;
		if(!player->cur_wpn->can_shoot)
		{
			float shot_perc = player->cur_wpn->shot_break_time_elapsed / player->cur_wpn->shot_break;
			if(shot_perc < 0.5f)
			{
				player->cur_wpn->offset = shot_perc;
			}
			else
			{
				player->cur_wpn->offset = 1.0f - shot_perc;
			}
		}
		float weapon_offset = player->cur_wpn->offset;
		player->cur_wpn->cir.center = (Vector2){ .x = player->pos.x + player->size.x/2 + pointer_dir.x * 64 -
													  pointer_dir.x * 32 * weapon_offset,
												 .y = player->pos.y + player->size.y/2 + pointer_dir.y * 64 -
												 	  pointer_dir.y * 32 * weapon_offset};


		update_weapon(player->cur_wpn);

		if(player->cur_wpn->type == MINIGUN && !player->cur_wpn->can_shoot)
		{
			move_speed_divisor = 0.2f;
		}

		if(IsKeyPressed(KEY_Q))
		{
			remove_from_inventory(player);
		}
	
	}

	// MOVE PLAYER
	player->move_dir = vec2_normalize(input);
	player->pos.x += player->move_dir.x * move_speed_divisor * player->speed * GetFrameTime();
	player->pos.y += player->move_dir.y * move_speed_divisor * player->speed * GetFrameTime();

	if (player->pos.x < 0) player->pos.x = 0;
	else if (player->pos.x + player->size.x > max_frame.x) player->pos.x = max_frame.x - player->size.x;
	if (player->pos.y < 0) player->pos.y = 0;
	else if (player->pos.y + player->size.y > max_frame.y) player->pos.y = max_frame.y - player->size.y;

	// TEST PURPOSE
	if(DEBUG)
	{
		if(IsMouseButtonPressed(1)) player->cur_hp = player->max_hp;
	}

	
	// UI
	char clr_mlpr = 2;
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
	
	//SHADOW//
	if(IS_SHADOWED) {
		DrawRectangle(pl_rec.x + SHADOW_OFFSET, pl_rec.y + SHADOW_OFFSET, pl_rec.width, pl_rec.height, SHADOW_COLOR);
	}
	//SHADOW//

	DrawTexturePro(player->tex, (Rectangle){ .x = 0, .y = 0, .width = player->tex.width * tx_multiplier, .height = player->tex.height },
					pl_rec, (Vector2){ 0 }, 0, WHITE);
}

Rectangle get_player_rec(Player* player)
{
	return (Rectangle){ .x = player->pos.x, .y = player->pos.y, .width = player->size.x, .height = player->size.y };
}

void add_to_inventory(Player* player, Weapon* weapon)
{
	if (weapon->is_in_inventory) return;
	if (player->inventory->slots[player->inventory->cur_slot_i]->is_empty) {
		int i = player->inventory->cur_slot_i;
		weapon->is_in_inventory = true;
		player->inventory->slots[i]->is_empty = false;
		player->inventory->slots[i]->weapon = weapon;
		player->cur_wpn = weapon;
		weapon->is_selected = true;
		return;
	}
	for (int i = 0; i < player->inventory->SLOT_COUNT; i++) {
		if (player->inventory->slots[i]->is_empty) {
			weapon->is_in_inventory = true;
			player->inventory->slots[i]->weapon = weapon;
			player->inventory->slots[i]->is_empty = false;
			return;
		}
	}
}

void remove_from_inventory(Player* player)
{
	player->cur_wpn->is_selected = false;
	player->cur_wpn->is_in_inventory = false;
	player->cur_wpn->cur_reload_time = 0;
	player->cur_wpn->is_reloading = false;
	player->cur_wpn->dir = (Vector2){ 0 };
	player->inventory->slots[player->inventory->cur_slot_i]->is_empty = true;
	player->inventory->slots[player->inventory->cur_slot_i]->weapon = (void*)0;
	player->cur_wpn = (void*)0;
}

void change_slot(Player* player)
{
	// select inventory slot
	int key = GetKeyPressed();
	if (key >= KEY_ONE && key <= KEY_NINE) {
		int slot_index = key - 48 - /*FOR INDEXING -->*/1;
	//	printf("%d\n", slot_index);
		if (slot_index < player->inventory->SLOT_COUNT) {
			if (player->cur_wpn)
				player->cur_wpn->is_selected = false;

			player->cur_wpn = player->inventory->slots[slot_index]->weapon;
	// ASSIGN PREVIOUS SELECTED SLOT TO UNSELECTED
			player->inventory->slots[player->inventory->cur_slot_i]->is_selected = false;			
	// ASSIGN is_selected TO TRUE FOR CURRENT SLOT
			player->inventory->cur_slot_i = slot_index;
			player->inventory->slots[slot_index]->is_selected = true;
			
			if (player->cur_wpn)
				player->cur_wpn->is_selected = true;
		}
	}

	int delta_mouse = (int)GetMouseWheelMove();
	if (delta_mouse == 0) return;

	if (player->cur_wpn)
		player->cur_wpn->is_selected = false;

	int i = player->inventory->cur_slot_i;
	int slot_index = i + delta_mouse;
	if (slot_index > player->inventory->SLOT_COUNT - 1) {
		slot_index = 0;
	} else if (slot_index < 0) {
		slot_index = player->inventory->SLOT_COUNT - 1;
	}

	player->cur_wpn = player->inventory->slots[slot_index]->weapon;
// ASSIGN PREVIOUS SELECTED SLOT TO UNSELECTED
	player->inventory->slots[player->inventory->cur_slot_i]->is_selected = false;			
// ASSIGN is_selected TO TRUE FOR CURRENT SLOT
	player->inventory->cur_slot_i = slot_index;
	player->inventory->slots[slot_index]->is_selected = true;
			
	if (player->cur_wpn)
		player->cur_wpn->is_selected = true;
}