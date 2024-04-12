#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include "include/game.h"
#include "include/utils.h"
#include "include/player.h"
#include "include/weapon.h"
#include "include/enemy.h"

Player* player = (void*)0;
Weapon* weapon = (void*)0;

unsigned int ENEMY_COUNT = 6;
Enemy* enemies[0];

void init()
{
	srand(time(0));

	player = init_player((Vector2){ 400, 300 }, (Vector2){ 64, 64 }, 300, 100);
	weapon = init_weapon((Circle){ .center = (Vector2){ 200, 200 }, .radius = 24 }, 128, 16, 2.5f, 10);
	//enemy = init_enemy((Vector2){ 0, 80 }, (Vector2){ 64, 96 }, 40, 15, 100);

	for(int i = 0; i < ENEMY_COUNT; i++)
	{
		enemies[i] = init_enemy((Vector2){ .x = rand() % WIDTH, .y = rand() % HEIGHT }, (Vector2){ .x = 64, .y = 64 },
											50, 40, rand() % 50 + 25 );
	}
}

void update()
{	
	update_player(player);

	if(!weapon->is_picked_up && is_rec_circle_colliding(get_player_rec(player), weapon->cir))
	{
		if(IsKeyPressed(KEY_E))
		{
			weapon->is_picked_up = true;
			player->cur_wpn = weapon;
		}
	}

	for(int i = 0; i < weapon->bullet_count; i++)
	{
		if(!weapon->bullets[i]->is_active) continue;

		update_bullet(weapon->bullets[i]);
	}

	Vector2 dest = { .x = player->pos.x + player->size.x/2, .y = player->pos.y + player->size.y/2 };

	for(int i = 0; i < ENEMY_COUNT; i++)
	{
		if(enemies[i]->is_dead) continue;

		for(int j = 0; j < weapon->bullet_count; j++)
		{
			if(!weapon->bullets[j]->is_active) continue;
			
			if(is_rec_circle_colliding(get_enemy_rec(enemies[i]), weapon->bullets[j]->cir))
			{
				enemies[i]->cur_hp -= weapon->damage;

				if(enemies[i]->cur_hp <= 0)
				{
					// kill enemy
					enemies[i]->cur_hp = 0;
					enemies[i]->is_dead = true;
				}
				weapon->bullets[j]->is_active = false;
			}
		}

		Vector2 enemy_mid = { .x = enemies[i]->pos.x + enemies[i]->size.x/2, .y = enemies[i]->pos.y + enemies[i]->size.y/2 };

		if(vec2_distance(dest, enemy_mid) > 32)
		{
			enemies[i]->dir = vec2_normalize((Vector2){ dest.x - enemy_mid.x, dest.y - enemy_mid.y });

			//enemy->pos.x = player->pos.x + player->size.x/2 - enemy->size.x/2;
			//enemy->pos.y = player->pos.y + player->size.y/2 - enemy->size.y/2;
		}
		else
		{
			enemies[i]->dir = (Vector2){ 0 };
		}

		if(CheckCollisionRecs(get_player_rec(player), get_enemy_rec(enemies[i])))
		{
			player->cur_hp -= enemies[i]->damage * GetFrameTime();
		}
		update_enemy(enemies[i]);

		if(enemy_mid.y > player->pos.y + player->size.y/2)
		{
			enemies[i]->sprite_order = 2;
		}
		else
		{
			enemies[i]->sprite_order = 0;
		}
	}
}

void late_update()
{
	for(int i = 0; i < 3; i++)
	{
		if(player->sprite_order == i)
		{
			draw_player(player);
			continue;
		}
		for(int j = 0; j < ENEMY_COUNT; j++)
		{
			if(enemies[j]->is_dead || enemies[j]->sprite_order != i) continue;
			draw_enemy(enemies[j]);
		}
	}
	draw_weapon(weapon);

	//UI

	//player hp
	Rectangle pl_hp_bar = { .x = 16, .y = 16, .width = 192, .height = 32 };
	DrawRectangleRec(pl_hp_bar, RED);
	DrawRectangle(pl_hp_bar.x, pl_hp_bar.y, pl_hp_bar.width * player->cur_hp / player->max_hp, pl_hp_bar.height, GREEN);
	DrawRectangleLinesEx(pl_hp_bar, 6, BLACK);

	// weapon ui
	if(player->cur_wpn)
	{
		Weapon* wpn = player->cur_wpn;

		char wpn_text[32];
		sprintf(wpn_text, "Ammo: %d/%d", wpn->cur_ammo, wpn->max_ammo);
		DrawText(wpn_text, 256, 0, 64, BLACK);
	}
}

void finish()
{
	free(player);
	for(int i = 0; i < weapon->bullet_count; i++)
	{
		free(weapon->bullets[i]);
	}
	free(weapon->bullets);
	free(weapon);
	for(int i = 0; i < ENEMY_COUNT; i++)
	{
		free(enemies[i]);
	}
}