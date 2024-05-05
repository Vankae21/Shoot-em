#include "include/gamecamera.h"
#include "include/game.h"
#include <stdlib.h>
#include <math.h>

GameCamera* init_camera(Vector2 offset, Player* player)
{
    GameCamera* gcamera = calloc(1, sizeof(GameCamera));
    gcamera->camera = calloc(1, sizeof(Camera2D));
    gcamera->player = player;
    gcamera->camera->target = vec2_sum(player->pos, vec2_div(player->size, 2));
    gcamera->camera->offset = offset;
    gcamera->camera->zoom = 1.0f;
    gcamera->camera->rotation = 0;
    gcamera->shake_sin = 0;
    gcamera->shake_time = 0;
    gcamera->is_shaking = false;

    return gcamera;
}

void update_camera(GameCamera* gcamera, Vector2 max_frame)
{
    Vector2 target = vec2_sum(gcamera->player->pos, vec2_div(gcamera->player->size, 2));

    unsigned short max_shake = 200, min_shake = 100;
    float shake_arg = 50;
    float shake_rate_x = sinf(gcamera->shake_time * shake_arg) * (float)(rand() % (max_shake - min_shake) + min_shake),
          shake_rate_y = sinf(gcamera->shake_time * shake_arg) * (float)(rand() % (max_shake - min_shake) + min_shake);
    if(rand() % 100 > 50)
    {
        shake_rate_x *= -1;
    }
    if(rand() % 100 > 50)
    {
        shake_rate_y *= -1;
    }
    Vector2 mod_target = { target.x + shake_rate_x, target.y + shake_rate_y };

    if(mod_target.x - WIDTH/2 < 0)
	{
		mod_target.x = WIDTH/2;
	}
	else if(mod_target.x + WIDTH/2 > max_frame.x)
	{
		mod_target.x = max_frame.x - WIDTH/2;
	}
	if(mod_target.y - HEIGHT/2 < 0)
	{
		mod_target.y = HEIGHT/2;
	}
	else if(mod_target.y + HEIGHT/2 > max_frame.y)
	{
		mod_target.y = max_frame.y - HEIGHT/2;
	}

	gcamera->camera->target = vec2_lerp(gcamera->camera->target, mod_target, 2.5f);
    
    if(!gcamera->is_shaking) return;
    gcamera->shake_time += GetFrameTime();
    if(gcamera->shake_time > 0.1f)
    {
        gcamera->is_shaking = false;
        gcamera->shake_time = 0;
    }
}