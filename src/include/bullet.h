#ifndef BULLET_H
#define BULLET_H

#include <raylib.h>
#include "utils.h"

typedef struct
{
	Circle cir;
	float speed;
	Vector2 dir;
	bool is_active;
} Bullet;

Bullet** init_bullets(unsigned int count, float speed, float radius);
void update_bullet(Bullet* bullet);
void draw_bullet(Bullet* bullet);

#endif