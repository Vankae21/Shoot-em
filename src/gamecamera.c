#include "include/gamecamera.h"
#include <stdlib.h>

GameCamera* init_camera(Vector2 offset, Vector2 target)
{
    GameCamera* gamecamera = calloc(1, sizeof(GameCamera));
    gamecamera->camera = calloc(1, sizeof(Camera2D));
    gamecamera->camera-> target = target;
    gamecamera->camera->offset = offset;
    gamecamera->camera->zoom = 1.0f;
    gamecamera->camera->rotation = 0;

    return gamecamera;
}

void update_camera(GameCamera* gamecamera, Vector2 target)
{
	gamecamera->camera->target = vec2_lerp(gamecamera->camera->target, target, 2.5f);
}