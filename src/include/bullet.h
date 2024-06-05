#ifndef BULLET_H
#define BULLET_H

#include <raylib.h>
#include "vutils.h"

#define BULLET_MAX_TIME 4.0f

typedef struct
{
	enum
	{
		PISTOL_BULLET,
		LASER
	} type;

	Circle cir;
	float speed;
	Vector2 dir;
	bool is_active;
	float time_to_alive;
} Bullet;

Bullet** init_bullets(int type, unsigned int count, float speed, float radius);
void update_bullet(Bullet* bullet);
void draw_bullet(Bullet* bullet, Texture2D tex);

#endif