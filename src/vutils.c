#include "include/vutils.h"
#include <stdlib.h>

int str_to_int(const char* str)
{
	int r = 0;
	for(int i = 0; str[i] != '\0'; i++)
	{
		r = r * 10 + str[i] - 48;
	}
	return r;
}

float vec2_len(Vector2 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

Vector2 vec2_normalize(Vector2 vec)
{
	return vec2_len(vec) == 0 ? (Vector2){0} : (Vector2){ .x = vec.x/vec2_len(vec), .y = vec.y/vec2_len(vec) };
}

bool is_rec_circle_colliding(Rectangle rec, Circle cir)
{
	Vector2 test_v = { .x = clamp(cir.center.x, rec.x, rec.x + rec.width), .y = clamp(cir.center.y, rec.y, rec.y + rec.height) };

	Vector2 dis = { .x = cir.center.x - test_v.x, .y = cir.center.y - test_v.y};

	if(vec2_len(dis) > cir.radius)
		return false;
	
	return true;
}

float vec2_distance(Vector2 vec_a, Vector2 vec_b)
{
	return sqrtf((vec_a.x - vec_b.x) * (vec_a.x - vec_b.x) + (vec_a.y - vec_b.y) * (vec_a.y - vec_b.y));
}

float clamp(float value, float min, float max)
{
	if(value > min && value < max)
	{
		return value;
	}
	return value < min ? min : max;
}

Vector2 vec2_lerp(Vector2 start_vec, Vector2 end_vec, float t)
{
	Vector2 diff = { .x = end_vec.x - start_vec.x, .y = end_vec.y - start_vec.y };
	return (Vector2){ .x = start_vec.x + diff.x * t * GetFrameTime(), .y = start_vec.y + diff.y * t * GetFrameTime() };
}

Vector2 vec2_translate(Vector2 start_vec, Vector2 end_vec, float t)
{
	Vector2 dir = vec2_normalize((Vector2){ .x = end_vec.x - start_vec.x, .y = end_vec.y - start_vec.y });
	return vec2_distance(start_vec, end_vec) > t * GetFrameTime() ?
			(Vector2){ .x = start_vec.x + dir.x * t * GetFrameTime(), .y = start_vec.y + dir.y * t * GetFrameTime() } : end_vec;
}

bool vec2_cmp(Vector2 a, Vector2 b)
{
	return (a.x == b.x && a.y == b.y);
}

Vector2 vec2_sum(Vector2 a, Vector2 b)
{
	return (Vector2){ a.x + b.x, a.y + b.y };
}

Vector2 vec2_div(Vector2 a, float divisor)
{
	return (Vector2){ a.x/divisor, a.y/divisor };
}

float random_val(float min, float max)
{
    // Generate a random float between 0 and 1
    float random_0_1 = (float)rand() / RAND_MAX;

    // Scale and shift the range to [min, max)
    float range = max - min;
    return min + random_0_1 * range;
}