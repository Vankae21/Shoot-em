#include "include/vutils.h"

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