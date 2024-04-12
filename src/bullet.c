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

		bullets[i]->cir = (Circle){ .center = {0}, .radius = radius };
		bullets[i]->speed = speed;
		bullets[i]->dir = (Vector2){ 0, 0 };
		bullets[i]->is_active = false;
	}

	return bullets;
}

void update_bullet(Bullet* bullet)
{
	bullet->cir.center.x += bullet->dir.x * bullet->speed * GetFrameTime();
	bullet->cir.center.y += bullet->dir.y * bullet->speed * GetFrameTime();
}

void draw_bullet(Bullet* bullet)
{
	DrawCircleV(bullet->cir.center, bullet->cir.radius, ORANGE);
}