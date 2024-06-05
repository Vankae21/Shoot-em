#ifndef GAMECAMERA_H
#define GAMECAMERA_H

#include <raylib.h>
#include "vutils.h"
#include "player.h"

typedef struct
{
    Camera2D* camera;
    Player* player;
    float shake_sin;
    float shake_time;
    bool is_shaking;
} GameCamera;

GameCamera* init_camera(Player* player);
void update_camera(GameCamera* gamecamera, Vector2 max_frame);

#endif