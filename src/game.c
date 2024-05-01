#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include "include/game.h"
#include "include/vutils.h"
#include "include/player.h"
#include "include/weapon.h"
#include "include/enemy.h"
#include "include/collectible.h"

unsigned int ENEMY_COUNT = 5;
unsigned int collected_coin = 0;
unsigned int COIN_COUNT = 0;

Font font;
Texture2D bg;
Texture2D collectible_spsheet;
Player* player = (void*)0;
Weapon* weapon = (void*)0;
Camera2D* camera = (void*)0;
Collectible** coins = (void*)0;
Enemy** enemies;




void init()
{
	srand(time(0));
	// ASSETS
	font = LoadFontEx("assets/JOYSTIX_MONOSPACE.otf", 64, (void*)0, 300);
	bg = LoadTexture("assets/bg.png");
	collectible_spsheet = LoadTexture("assets/coin.png");

	player = init_player((Vector2){ WIDTH/2, HEIGHT/2 }, (Vector2){ 64, 64 }, 300, 100, "assets/player.png");
	weapon = init_weapon((Circle){ .center = (Vector2){ 200, 200 }, .radius = 16 }, 32, 16, 2.5f, 10, "assets/weapon.png", "assets/bullet.png");
	coins = calloc(COIN_COUNT, sizeof(Collectible*));
	for(int i = 0; i < COIN_COUNT; i++)
	{
		coins[i] = init_collectible(COIN, (Vector2){ rand() % WIDTH, rand() % HEIGHT });
	}
	enemies = calloc(ENEMY_COUNT, sizeof(Enemy*));
	for(int i = 0; i < ENEMY_COUNT; i++)
	{
		enemies[i] = init_enemy((Vector2){ .x = rand() % WIDTH, .y = rand() % HEIGHT }, (Vector2){ .x = 64, .y = 64 },
											50, 40, rand() % 50 + 25 , "assets/zombie.png");
	}

	camera = (Camera2D*)malloc(sizeof(Camera2D));
	camera->target = (Vector2){ .x = player->pos.x + player->size.x/2, .y = player->pos.y + player->size.y/2 };
	camera->offset = (Vector2){ .x = WIDTH/2, .y = HEIGHT/2 };
	camera->zoom = 1.0f;
	camera->rotation = 0;
}

void update()
{	
	camera->zoom += GetMouseWheelMove() * GetFrameTime();
	update_player(player);
	Vector2 player_dest = { .x = player->pos.x + player->size.x/2, .y = player->pos.y + player->size.y/2 };

	Vector2 player_cam_pos = player_dest;

	if(player->pos.x + player->size.x/2 - WIDTH/2 < 0)
	{
		player_cam_pos.x = WIDTH/2;
	}
	else if(player->pos.x + player->size.x/2 + WIDTH/2 > bg.width * SIZE_MULTIPLIER)
	{
		player_cam_pos.x = bg.width * SIZE_MULTIPLIER - WIDTH/2;
	}
	if(player->pos.y + player->size.y/2 - HEIGHT/2 < 0)
	{
		player_cam_pos.y = HEIGHT/2;
	}
	else if(player->pos.y + player->size.y/2 + HEIGHT/2 > bg.height * SIZE_MULTIPLIER)
	{
		player_cam_pos.y = bg.height * SIZE_MULTIPLIER - HEIGHT/2;
	}

	camera->target = vec2_lerp(camera->target, player_cam_pos, 2.5f);


	if(!weapon->is_picked_up && is_rec_circle_colliding(get_player_rec(player), weapon->cir))
	{
		DrawText("Press E to pick up", weapon->cir.center.x - weapon->cir.radius, weapon->cir.center.y - weapon->cir.radius - 32, 32, BLACK);
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


	// CHECK IF PLAYER IS TAKING DAMAGE
	bool player_is_taking_dmg = false;

	for(int i = 0; i < ENEMY_COUNT; i++)
	{
		if(enemies[i]->is_dead) continue;
		update_enemy(enemies[i]);
		
		Vector2 enemy_mid = { .x = enemies[i]->pos.x + enemies[i]->size.x/2, .y = enemies[i]->pos.y + enemies[i]->size.y/2 };

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
					unsigned short dropped_coin_count = rand() % ENEMY_MAX_COIN_DROP + 1;
					for(int i = 0; i < dropped_coin_count; i++)
					{
						add_collectible(init_collectible(COIN, enemy_mid), &coins, &COIN_COUNT);
					}
				}
				weapon->bullets[j]->is_active = false;
			}
		}


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

	for(int i = 0; i < COIN_COUNT; i++)
	{
		// COLLECT COIN
		if(is_rec_circle_colliding(get_player_rec(player), coins[i]->cir))
		{
			collected_coin++;
			coins[i]->is_collected = true;
			rerow_collectibles(&coins, &COIN_COUNT);
		}
	}

	// ASSIGN TO PLAYER EVERYTIME
	player->is_taking_damage = player_is_taking_dmg;
}

void late_update()
{
	BeginMode2D(*camera);
	DrawTextureEx(bg, (Vector2){ 0 }, 0, SIZE_MULTIPLIER, WHITE);

	for(int i = 0; i < COIN_COUNT; i++)
	{
		draw_collectible(coins[i], collectible_spsheet);
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
			if(enemies[j]->is_dead || enemies[j]->sprite_order != i) continue;
			draw_enemy(enemies[j]);
		}
	}
	draw_weapon(weapon);
	EndMode2D();
}

void draw_ui()
{
	//UI
	
	// PLAYER DMG SCR
	DrawRectangle(0, 0, WIDTH, HEIGHT, player->dmg_color);

	//player UI
	Rectangle pl_hp_bar = { .x = 16, .y = 16, .width = 192, .height = 32 };
	DrawRectangleRec(pl_hp_bar, RED);
	DrawRectangle(pl_hp_bar.x, pl_hp_bar.y, pl_hp_bar.width * player->cur_hp / player->max_hp, pl_hp_bar.height, GREEN);
	DrawRectangleLinesEx(pl_hp_bar, 6, BLACK);

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