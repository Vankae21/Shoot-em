#include "include/weapon.h"
#include "include/bullet.h"
#include "include/game.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

Weapon* init_weapon(unsigned short type, Vector2 pos, const char* bullet_tex_path)
{
	Weapon* weapon = calloc(1, sizeof(Weapon));

	// defaults
	weapon->type = type;
	weapon->is_picked_up = false;
	weapon->cir = (Circle){ .center = pos, .radius = 16 };
	weapon->dir = (Vector2){ 0, 0 };
	weapon->is_reloading = false;
	weapon->cur_reload_time = 0;

	switch(type)
	{
		case PISTOL:
			weapon->bullet_count = 32;
			weapon->bullets = init_bullets(weapon->bullet_count, 1000, 8);
			weapon->max_ammo = 16;
			weapon->reload_time = 2.5f;
			weapon->damage = 10;
	}

	weapon->cur_ammo = weapon->max_ammo;
	weapon->bullet_tex = LoadTexture(bullet_tex_path);

	return weapon;
}

void update_weapon(Weapon* weapon)
{
	// reload ammo
	if(weapon->is_reloading)
	{
		if(weapon->cur_reload_time < weapon->reload_time)
		{
			weapon->cur_reload_time += GetFrameTime();
			if(weapon->cur_reload_time > weapon->reload_time)
			{
				weapon->is_reloading = false;
				weapon->cur_ammo = weapon->max_ammo;
				weapon->cur_reload_time = 0;
			}
		}
	}

	// trigger reload
	if((IsKeyPressed(KEY_R) && weapon->cur_ammo != weapon->max_ammo)
	//	|| (IsMouseButtonPressed(0) && weapon->cur_ammo == 0)
		)
	{
		weapon->is_reloading = true;
	}

	// shoot ammo
	if(!weapon->is_reloading && weapon->cur_ammo > 0 && IsMouseButtonPressed(0))
	{
		for(int i = 0; i < weapon->bullet_count; i++)
		{
			if(weapon->bullets[i]->is_active) continue;
				
			weapon->bullets[i]->is_active = true;
			weapon->bullets[i]->cir.center = weapon->cir.center;
			weapon->bullets[i]->dir = weapon->dir;
			weapon->cur_ammo--;
			break;
		}	
	}
}

void draw_weapon(Weapon* weapon, Texture2D weapon_spsheet)
{
	if(DEBUG)
	DrawCircleV(weapon->cir.center, weapon->cir.radius, BROWN);


	float degree = atan2f(weapon->dir.y, weapon->dir.x) * RAD2DEG;
	float ty_mltp = fabs(degree) > 90 ? -1 : 1;

	DrawTexturePro(weapon_spsheet, (Rectangle){ .x = weapon->type * 16, .y = 0, .width = 16, .height = 16 * ty_mltp },
					(Rectangle){ .x = weapon->cir.center.x, .y = weapon->cir.center.y,
					 			.width = weapon->cir.radius * 2, .height = weapon->cir.radius * 2 },
					(Vector2){ weapon->cir.radius, weapon->cir.radius }, degree, WHITE);

	for(int i = 0; i < weapon->bullet_count; i++)
	{
		if(!weapon->bullets[i]->is_active) continue;

		draw_bullet(weapon->bullets[i], weapon->bullet_tex);
	}
	if(weapon->is_reloading)
	{
		int bar_x = weapon->cir.center.x - weapon->cir.radius;
		int bar_y = weapon->cir.center.y - weapon->cir.radius - 16;
		int bar_width = weapon->cir.radius * 2; int bar_height = 8;
		DrawRectangle(bar_x, bar_y, bar_width, bar_height, RED);
		DrawRectangle(bar_x, bar_y, bar_width * weapon->cur_reload_time / weapon->reload_time, bar_height, GREEN);
	}
}