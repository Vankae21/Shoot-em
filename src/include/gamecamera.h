#ifndef GAMECAMERA_H
#define GAMECAMERA_H

#include <raylib.h>
#include "vutils.h"

typedef struct
{
    Camera2D* camera;
} GameCamera;

GameCamera* init_camera(Vector2 offset, Vector2 target);
void update_camera(GameCamera* camera, Vector2 target);

#endif