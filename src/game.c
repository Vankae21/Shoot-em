#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include <float.h>
#include "include/game.h"
#include "include/vutils.h"
#include "include/player.h"
#include "include/weapon.h"
#include "include/enemy.h"
#include "include/collectible.h"
#include "include/gamecamera.h"

int WIDTH = 1280, HEIGHT = 720;

bool IS_FULLSCREEN = false;
bool IS_SHADOWED = true;
bool is_paused = false;

unsigned int collected_coin = 0;
unsigned int kill_count = 0;
unsigned int ENEMY_COUNT = 3;
unsigned int COLLECTIBLE_COUNT = 0;
unsigned int WEAPON_COUNT = 1;

Font font;

// BACKGROUND
Texture2D bg;
Rectangle bg_rec;
//--------------------
Texture2D collectible_spsheet;
Texture2D enemies_spsheet;
Texture2D weapon_spsheet;
Texture2D hpbar_spsheet;
Player* player = (void*)0;
Weapon** weapons = (void*)0;
GameCamera* gamecamera = (void*)0;
Collectible** collectibles = (void*)0;
Enemy** enemies = (void*)0;

void init()
{
	srand(time(0));
	// ASSETS
	font = LoadFontEx("assets/JOYSTIX_MONOSPACE.otf", 64, (void*)0, 300);
	// BACKGROUND
	bg = LoadTexture("assets/bg.png");
	bg_rec = (Rectangle){ 0, 0, bg.width * 2, bg.height * 2 };
	//------------------------------------------------------------------
	collectible_spsheet = LoadTexture("assets/collectible_spsheet.png");
	enemies_spsheet = LoadTexture("assets/zombie.png");
	weapon_spsheet = LoadTexture("assets/weapon_spsheet.png");
	hpbar_spsheet = LoadTexture("assets/hpbar.png");

	player = init_player((Vector2){ WIDTH/2, HEIGHT/2 }, (Vector2){ 64, 64 }, 300, 100, "assets/player.png");
	collectibles = calloc(COLLECTIBLE_COUNT, sizeof(Collectible*));

	for(int i = 0; i < COLLECTIBLE_COUNT; i++)
	{
		collectibles[i] = init_collectible(rand() % 2, (Vector2){ rand() % WIDTH, rand() % HEIGHT });
	}
	enemies = calloc(ENEMY_COUNT, sizeof(Enemy*));
	for(int i = 0; i < ENEMY_COUNT; i++)
	{
		enemies[i] = init_enemy(ZOMBIE, (Vector2){ .x = rand() % WIDTH, .y = rand() % HEIGHT }, (Vector2){ .x = 64, .y = 64 },
											50, 40, rand() % 150 + 50);
	}
	weapons = calloc(WEAPON_COUNT, sizeof(Weapon*));
	for(int i = 0; i < WEAPON_COUNT; i++)
	{
		weapons[i] = init_weapon(PISTOL, (Vector2){ rand() % WIDTH, rand() % HEIGHT }, "assets/bullet.png");
	}

	gamecamera = init_camera(player);
	player->cur_wpn = weapons[0];
}

void update()
{	
	if(IsKeyPressed(KEY_P))
	{
		is_paused = is_paused ? 0 : 1;
	}
	if(is_paused) return;

	Vector2 max_frame = { bg_rec.width, bg_rec.height };

	gamecamera->camera->zoom += GetMouseWheelMove() * GetFrameTime();
	update_player(player, max_frame);
	Vector2 player_dest = vec2_sum(player->pos, vec2_div(player->size, 2));

	update_camera(gamecamera, max_frame);

	// CHECK IF PLAYER IS TAKING DAMAGE
	bool player_is_taking_dmg = false;

	for(int i = 0; i < ENEMY_COUNT; i++)
	{
		update_enemy(enemies[i]);
		
		Vector2 enemy_mid = { .x = enemies[i]->pos.x + enemies[i]->size.x/2, .y = enemies[i]->pos.y + enemies[i]->size.y/2 };

		if(vec2_distance(player_dest, enemy_mid) > 32)
		{
			enemies[i]->dir = vec2_normalize((Vector2){ player_dest.x - enemy_mid.x, player_dest.y - enemy_mid.y });
		}
		else
		{
			enemies[i]->dir = (Vector2){ 0 };
		}

		if(CheckCollisionRecs(get_player_rec(player), get_enemy_rec(enemies[i])))
		{
			player_is_taking_dmg = true;
			player->cur_hp -= enemies[i]->damage * GetFrameTime();
		}

		if(enemy_mid.y > player_dest.y)
		{
			enemies[i]->sprite_order = 2;
		}
		else
		{
			enemies[i]->sprite_order = 0;
		}
	}

	for(int w = 0; w < WEAPON_COUNT; w++)
	{
		if(!player->cur_wpn && is_rec_circle_colliding(get_player_rec(player), weapons[w]->cir))
		{
			if(IsKeyPressed(KEY_E))
			{
				weapons[w]->is_picked_up = true;
				player->cur_wpn = weapons[w];
			}
		}
		for(int i = 0; i < weapons[w]->bullet_count; i++)
		{
			if(!weapons[w]->bullets[i]->is_active) continue;

			update_bullet(weapons[w]->bullets[i]);
		}


		for(int j = 0; j < weapons[w]->bullet_count; j++)
		{
			if(!weapons[w]->bullets[j]->is_active) continue;
			float distance_nearest_enemy = FLT_MAX;
			Enemy* enemy_nearest_collided = (void*)0;
			Vector2 knockback_pos;
			float damage;
			for(int i = 0; i < ENEMY_COUNT; i++)
			{
				Vector2 enemy_mid = get_enemy_mid(enemies[i]);
			
				if(is_rec_circle_colliding(get_enemy_rec(enemies[i]), weapons[w]->bullets[j]->cir))
				{
					if(vec2_distance(player_dest, enemy_mid) < distance_nearest_enemy)
					{
						float knockback = 20;
						distance_nearest_enemy = vec2_distance(player_dest, enemy_mid);
						enemy_nearest_collided = enemies[i];
						knockback_pos = (Vector2){ enemies[i]->pos.x + knockback * weapons[w]->bullets[j]->dir.x,
										enemies[i]->pos.y + knockback * weapons[w]->bullets[j]->dir.y };
						damage = weapons[w]->damage;
					}
				}
			}
			if(enemy_nearest_collided)
			{
				Vector2 enemy_mid = get_enemy_mid(enemy_nearest_collided);
				enemy_nearest_collided->cur_hp -= damage;
				enemy_nearest_collided->knockback_complete = false;
				enemy_nearest_collided->knockback_pos = knockback_pos;
				if(enemy_nearest_collided->cur_hp <= 0)
				{
					// kill enemy
					kill_count++;
					enemy_nearest_collided->cur_hp = 0;
					enemy_nearest_collided->is_dead = true;
					rerow_enemies(&enemies, &ENEMY_COUNT);
					unsigned short dropped_coin_count = rand() % ENEMY_MAX_COIN_DROP + 1;
					for(int i = 0; i < dropped_coin_count; i++)
					{
						add_collectible(init_collectible(COIN, enemy_mid), &collectibles, &COLLECTIBLE_COUNT);
					}
					float medkit_chance = (float)(rand() % 101)/100.0f;
					if(medkit_chance <= ENEMY_MEDKIT_DROP_CHANCE)
					{
						add_collectible(init_collectible(MEDKIT, enemy_mid), &collectibles, &COLLECTIBLE_COUNT);
					}
				}
				weapons[w]->bullets[j]->is_active = false;
			}
			
		}
	}

	for(int i = 0; i < COLLECTIBLE_COUNT; i++)
	{
		// COLLECT ITEMS
		if(is_rec_circle_colliding(get_player_rec(player), collectibles[i]->cir))
		{
			bool collected_item = false;
			switch(collectibles[i]->type)
			{
				case COIN:
					collected_coin++;
					collected_item = true;
					break;
				case MEDKIT:
					if(player->cur_hp == player->max_hp) break;
					player->cur_hp = player->max_hp;
					collected_item = true;
					break;
				default:
					printf("Unidentified type for collectible: %d\n", collectibles[i]->type);
					exit(1);
				
			}
			if(collected_item)
			{
				collectibles[i]->is_collected = true;
				rerow_collectibles(&collectibles, &COLLECTIBLE_COUNT);
			}
			
		}
	}

	// ASSIGN TO PLAYER EVERYTIME
	player->is_taking_damage = player_is_taking_dmg;
}

void late_update()
{
	BeginMode2D(*(gamecamera->camera));
	DrawTexturePro(bg, (Rectangle){0, 0, bg.width, bg.height}, bg_rec, (Vector2){0}, 0, WHITE);
	
	for(int i = 0; i < COLLECTIBLE_COUNT; i++)
	{
		if(!IS_SHADOWED) break;
		draw_collectible_shadow(collectibles[i]);
	}
	for(int i = 0; i < COLLECTIBLE_COUNT; i++)
	{
		draw_collectible(collectibles[i], collectible_spsheet);
	}
	for(int i = 0; i < ENEMY_COUNT; i++)
	{
		if(!IS_SHADOWED) break;
		draw_enemy_shadow(enemies[i]);
	}
	for(int i = 0; i < 3; i++)
	{
		if(player->sprite_order == i)
		{
			draw_player(player);
			continue;
		}
		for(int j = 0; j < ENEMY_COUNT; j++)
		{
			if(enemies[j]->sprite_order != i) continue;
			draw_enemy(enemies[j], enemies_spsheet);
		}
	}
	for(int i = 0; i < WEAPON_COUNT; i++)
	{
		draw_weapon(weapons[i], weapon_spsheet);
	}
	EndMode2D();
}

void draw_ui()
{
	//UI
	
	// PLAYER DMG SCR
	DrawRectangle(0, 0, WIDTH, HEIGHT, player->dmg_color);

	//player UI

	//hpbar source rec red
	Rectangle pl_hp_bar_red = { .x = 0, .y = 0, .width = 192, .height = 32 };
	//hpbar source rec green
	Rectangle pl_hp_bar_green = { .x = 0, .y = 32, .width = player->cur_hp / player->max_hp * 192, .height = 32 };
	//hpbar pos rec red
	Rectangle pl_hp_bar_red_dest = { .x = 16, .y = 16, .width = 192, .height = 32 };
	//hpbar pos rec green
	Rectangle pl_hp_bar_green_dest = { .x = 16, .y = 16, .width = player->cur_hp / player->max_hp * 192, .height = 32 };
	//red hp bar
	DrawTexturePro(hpbar_spsheet, pl_hp_bar_red, pl_hp_bar_red_dest, (Vector2){0}, 0, WHITE);
	//green hp bar
	DrawTexturePro(hpbar_spsheet, pl_hp_bar_green, pl_hp_bar_green_dest, (Vector2){0}, 0, WHITE);

	// COIN UI
	char coin_text[32];
	sprintf(coin_text, "Coins:%d", collected_coin);
	DrawTextEx(font, coin_text, (Vector2){ 16, 64 }, 32, 0, BLACK);

	// weapon ui
	if(player->cur_wpn)
	{
		Weapon* wpn = player->cur_wpn;

		char wpn_text[32];
		sprintf(wpn_text, "Ammo:%d/%d", wpn->cur_ammo, wpn->max_ammo);
		DrawTextEx(font, wpn_text, (Vector2){ 256, 0 }, 64, 0, BLACK);
	}

}

void finish()
{
	printf("Total colleted coins: %d\n", collected_coin);
	printf("Total killed enemies: %d\n", kill_count);
}