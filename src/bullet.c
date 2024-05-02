#include "include/bullet.h"
#include "include/weapon.h"
#include "include/game.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

Bullet** init_bullets(unsigned int count, float speed, float radius)
{
	Bullet** bullets = calloc(count, sizeof(Bullet*));

	for(int i = 0; i < count; i++)
	{
		bullets[i] = calloc(1, sizeof(Bullet));

		bullets[i]->type = PISTOL_BULLET;
		bullets[i]->cir = (Circle){ .center = {0}, .radius = radius };
		bullets[i]->speed = speed;
		bullets[i]->dir = (Vector2){ 0, 0 };
		bullets[i]->is_active = false;
		bullets[i]->time_to_alive = 0;
	}

	return bullets;
}

void update_bullet(Bullet* bullet)
{
	bullet->cir.center.x += bullet->dir.x * bullet->speed * GetFrameTime();
	bullet->cir.center.y += bullet->dir.y * bullet->speed * GetFrameTime();
	bullet->time_to_alive += GetFrameTime();

	if(bullet->time_to_alive > BULLET_MAX_TIME)
	{
		bullet->is_active = false;
		bullet->time_to_alive = 0;
	}
}

void draw_bullet(Bullet* bullet, Texture2D tex)
{
	if(DEBUG)
		DrawCircleV(bullet->cir.center, bullet->cir.radius, ORANGE);

	float rot = atan2f(bullet->dir.y, bullet->dir.x) * RAD2DEG;
	DrawTexturePro(tex, (Rectangle){ .x = 0, .y = 0, .width = tex.width, .height = tex.height },
					(Rectangle){ .x = bullet->cir.center.x, .y = bullet->cir.center.y, bullet->cir.radius*2, bullet->cir.radius*2 },
					(Vector2){ .x = bullet->cir.radius, .y = bullet->cir.radius }, rot, WHITE);
}