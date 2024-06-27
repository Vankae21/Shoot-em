#include "include/weapon.h"
#include "include/bullet.h"
#include "include/game.h"
#include "include/vutils.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

Weapon* init_weapon(unsigned short type, Vector2 pos, const char* bullet_tex_path)
{
	Weapon* weapon = calloc(1, sizeof(Weapon));

	// defaults
	weapon->type = type;
	weapon->is_selected = false;
	weapon->is_in_inventory = false;
	weapon->cir = (Circle){ .center = pos, .radius = 16 };
	weapon->dir = (Vector2){ 0, 0 };
	weapon->is_reloading = false;
	weapon->cur_reload_time = 0;
	weapon->shot_break_time_elapsed = 0;
	weapon->can_shoot = true;
	weapon->offset = 0;
	weapon->shot_offset = 0;

	switch(type)
	{
		case PISTOL:
			weapon->bullet_count = 32;
			weapon->bullets = init_bullets(PISTOL_BULLET, weapon->bullet_count, 1000, 8, PISTOL_BULLET_MAX_TIME);
			weapon->max_ammo = 16;
			weapon->reload_time = 2.5f;
			weapon->damage = 10;
			weapon->shot_break = 0.33f;
		//	weapon->shot_offset = 0;
			break;
		
		case MINIGUN:
			weapon->bullet_count = 400;
			weapon->bullets = init_bullets(PISTOL_BULLET, weapon->bullet_count, 1000, 8, PISTOL_BULLET_MAX_TIME);
			weapon->max_ammo = 200;
			weapon->reload_time = 10.0f;
			weapon->damage = 1;
			weapon->shot_break = 0.1f;
			weapon->shot_offset = 32;
			break;

		case SHOTGUN:

			/*
			    Bullet count: 8
			    Max ammo: 4
			    Reload time: 3f
			    Damage: 30
			    Shot break: 0.5f
			    Bullet speed: 1500
			    Bullet radius: 16
			*/
			
			weapon->bullet_count = 16;
			weapon->bullets = init_bullets(PISTOL_BULLET, weapon->bullet_count, 1500, 16, SHOTGUN_BULLET_MAX_TIME);
			weapon->max_ammo = 2;
			weapon->reload_time = 3.0f;
			weapon->damage = 30;
			weapon->shot_break = 0.5f;
			weapon->shot_offset = 32;
			break;

		default:
			break;
	}

	weapon->cur_ammo = weapon->max_ammo;
	weapon->bullet_tex = LoadTexture(bullet_tex_path);

	return weapon;
}

void update_weapon(Weapon* weapon)
{
	switch(weapon->type)
	{
		case PISTOL:
			// reload ammo
			if(weapon->is_reloading)
			{
				weapon->can_shoot = false;
				weapon->shot_break_time_elapsed = 0;
				if(weapon->cur_reload_time < weapon->reload_time)
				{
					weapon->cur_reload_time += GetFrameTime();
					if(weapon->cur_reload_time > weapon->reload_time)
					{
						weapon->is_reloading = false;
						weapon->can_shoot = true;
						weapon->cur_ammo = weapon->max_ammo;
						weapon->cur_reload_time = 0;
					}
				}
			}
			else if(!weapon->can_shoot)
			{
				weapon->shot_break_time_elapsed += GetFrameTime();
				if(weapon->shot_break_time_elapsed >= weapon->shot_break)
				{
					weapon->can_shoot = true;
					weapon->shot_break_time_elapsed = 0;
					weapon->offset = 0;
				}
			}

			// trigger reload
			if((IsKeyPressed(KEY_R) && weapon->cur_ammo != weapon->max_ammo))
			{
				weapon->is_reloading = true;
				weapon->can_shoot = false;
			}

			// shoot ammo
			if(weapon->can_shoot && weapon->cur_ammo > 0 && IsMouseButtonPressed(0))
			{
				weapon->can_shoot = false;
				gamecamera->is_shaking = true;
				for(int i = 0; i < weapon->bullet_count; i++)
				{
					if(weapon->bullets[i]->is_active) continue;
						
					weapon->bullets[i]->is_active = true;
					weapon->bullets[i]->cir.center = (Vector2){ .x = weapon->cir.center.x + weapon->shot_offset * weapon->dir.x,
																.y = weapon->cir.center.y + weapon->shot_offset * weapon->dir.y };
					weapon->bullets[i]->dir = weapon->dir;
					weapon->cur_ammo--;
					break;
				}	
			}
			break;

		case MINIGUN:
			// reload ammo
			if(weapon->is_reloading)
			{
				weapon->can_shoot = false;
				weapon->shot_break_time_elapsed = 0;
				if(weapon->cur_reload_time < weapon->reload_time)
				{
					weapon->cur_reload_time += GetFrameTime();
					if(weapon->cur_reload_time > weapon->reload_time)
					{
						weapon->is_reloading = false;
						weapon->can_shoot = true;
						weapon->cur_ammo = weapon->max_ammo;
						weapon->cur_reload_time = 0;
					}
				}
			}
			else if(!weapon->can_shoot)
			{
				weapon->shot_break_time_elapsed += GetFrameTime();
				if(weapon->shot_break_time_elapsed >= weapon->shot_break)
				{
					weapon->can_shoot = true;
					weapon->shot_break_time_elapsed = 0;
					weapon->offset = 0;
				}
			}

			// trigger reload
			if((IsKeyPressed(KEY_R) && weapon->cur_ammo != weapon->max_ammo))
			{
				weapon->is_reloading = true;
				weapon->can_shoot = false;
			}

			// shoot ammo
			if(weapon->can_shoot && weapon->cur_ammo > 0 && IsMouseButtonDown(0))
			{
				weapon->can_shoot = false;
				gamecamera->is_shaking = true;
				for(int i = 0; i < weapon->bullet_count; i++)
				{
					if(weapon->bullets[i]->is_active) continue;
						
					weapon->bullets[i]->is_active = true;
					weapon->bullets[i]->cir.center = (Vector2){ .x = weapon->cir.center.x + weapon->shot_offset * weapon->dir.x,
																.y = weapon->cir.center.y + weapon->shot_offset * weapon->dir.y };
					weapon->bullets[i]->dir = weapon->dir;
					weapon->cur_ammo--;
					break;
				}	
			}
			break;

		case SHOTGUN:
			// reload ammo
			if(weapon->is_reloading)
			{
				weapon->can_shoot = false;
				weapon->shot_break_time_elapsed = 0;
				if(weapon->cur_reload_time < weapon->reload_time)
				{
					weapon->cur_reload_time += GetFrameTime();
					if(weapon->cur_reload_time > weapon->reload_time)
					{
						weapon->is_reloading = false;
						weapon->can_shoot = true;
						weapon->cur_ammo = weapon->max_ammo;
						weapon->cur_reload_time = 0;
					}
				}
			}
			else if(!weapon->can_shoot)
			{
				weapon->shot_break_time_elapsed += GetFrameTime();
				if(weapon->shot_break_time_elapsed >= weapon->shot_break)
				{
					weapon->can_shoot = true;
					weapon->shot_break_time_elapsed = 0;
					weapon->offset = 0;
				}
			}

			// trigger reload
			if((IsKeyPressed(KEY_R) && weapon->cur_ammo != weapon->max_ammo))
			{
				weapon->is_reloading = true;
				weapon->can_shoot = false;
			}

			// shoot ammo
			int bullets_to_shoot = 4;
			if(weapon->can_shoot && weapon->cur_ammo > 0 && IsMouseButtonPressed(0))
			{
				weapon->can_shoot = false;
				gamecamera->is_shaking = true;
				for(int i = 0; i < weapon->bullet_count; i++)
				{
					if(weapon->bullets[i]->is_active) continue;
					if (bullets_to_shoot <= 0) {
						weapon->cur_ammo--;
						break;
					}

					weapon->bullets[i]->is_active = true;
					weapon->bullets[i]->cir.center = (Vector2){ .x = weapon->cir.center.x + weapon->shot_offset * weapon->dir.x,
																.y = weapon->cir.center.y + weapon->shot_offset * weapon->dir.y };
					// randomize directions
					float dir_limit = .2f;
					Vector2 randomized_dir = {random_val(weapon->dir.x - dir_limit, weapon->dir.x + dir_limit),
											random_val(weapon->dir.y - dir_limit, weapon->dir.y + dir_limit)};
					weapon->bullets[i]->dir = randomized_dir;
					bullets_to_shoot--;
				}	
			}
			break;

		default:
			break;
	}
}

void draw_weapon(Weapon* weapon, Texture2D weapon_spsheet)
{
	if (weapon->is_in_inventory && !weapon->is_selected) return;

	if (DEBUG) {
		DrawCircleV(weapon->cir.center, weapon->cir.radius, BROWN);		
	}


	float degree = atan2f(weapon->dir.y, weapon->dir.x) * RAD2DEG;
	float ty_mltp = fabs(degree) > 90 ? -1 : 1;

	DrawTexturePro(weapon_spsheet, (Rectangle){ .x = weapon->type * 32, .y = 0, .width = 32, .height = 16 * ty_mltp },
					(Rectangle){ .x = weapon->cir.center.x, .y = weapon->cir.center.y,
					 			.width = weapon->cir.radius * 4, .height = weapon->cir.radius * 2 },
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