#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <raylib.h>

typedef struct
{
	Vector2 center;
	float radius;
} Circle;

float vec2_len(Vector2 vec);
Vector2 vec2_normalize(Vector2 vec);
bool is_rec_circle_colliding(Rectangle rec, Circle cir);
float vec2_distance(Vector2 vec_a, Vector2 vec_b);
float clamp(float value, float min, float max);

#endif