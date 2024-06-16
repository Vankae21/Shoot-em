#ifndef VUTILS_H
#define VUTILS_H

#include <math.h>
#include <raylib.h>

typedef struct
{
	Vector2 center;
	float radius;
} Circle;

int str_to_int(const char* str);

float vec2_len(Vector2 vec);
Vector2 vec2_normalize(Vector2 vec);
bool is_rec_circle_colliding(Rectangle rec, Circle cir);
float vec2_distance(Vector2 vec_a, Vector2 vec_b);
float clamp(float value, float min, float max);
Vector2 vec2_lerp(Vector2 start_vec, Vector2 end_vec, float t);
Vector2 vec2_translate(Vector2 start_vec, Vector2 end_vec, float t);
bool vec2_cmp(Vector2 a, Vector2 b);
Vector2 vec2_sum(Vector2 a, Vector2 b);
Vector2 vec2_div(Vector2 a, float divisor);
float random_val(float min, float max);

#endif