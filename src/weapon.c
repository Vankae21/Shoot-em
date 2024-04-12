#include "include/weapon.h"
#include "include/bullet.h"
#include "include/game.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

Weapon* init_weapon(Circle cir, unsigned int bullet_count, unsigned int max_ammo, float reload_time, float damage)
{
	Weapon* weapon = calloc(1, sizeof(Weapon));

	weapon->is_picked_up = false;
	weapon->cir = cir;
	weapon->dir = (Vector2){ 0, 0 };
	weapon->bullet_count = bullet_count;
	weapon->bullets = init_bullets(bullet_count, 700, 16);
	weapon->max_ammo = max_ammo;
	weapon->cur_ammo = max_ammo;
	weapon->is_reloading = false;
	weapon->reload_time = reload_time;
	weapon->cur_reload_time = 0;
	weapon->damage = damage;

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

void draw_weapon(Weapon* weapon)
{
	DrawCircleV(weapon->cir.center, weapon->cir.radius, BROWN);

	for(int i = 0; i < weapon->bullet_count; i++)
	{
		if(!weapon->bullets[i]->is_active) continue;

		draw_bullet(weapon->bullets[i]);
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