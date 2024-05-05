#include "include/joystick.h"
#include "include/game.h"
#include <stdlib.h>
#include <stdio.h>

Joystick* init_joystick(Vector2 pos, float max_dist, float radius)
{
    Joystick* joystick = calloc(1, sizeof(Joystick));

    joystick->pos = pos;
    joystick->max_dist = max_dist;
    joystick->stick = (Circle){ .center = pos, .radius = radius };
    joystick->dir = (Vector2){ 0 };

    return joystick;
}

void update_joystick(Joystick* joystick)
{
    if(!IsMouseButtonDown(0) || GetMouseX() > WIDTH/2)
    {
        joystick->dir = (Vector2){ 0 };
        joystick->stick.center = joystick->pos;
        return;
    }

    Vector2 mousePos = GetMousePosition();

    Vector2 vec = (Vector2){ .x = mousePos.x - joystick->pos.x, .y = mousePos.y - joystick->pos.y };
    float dist = vec2_len(vec); dist = fminf(dist, joystick->max_dist);
    joystick->dir = vec2_normalize(vec);
    joystick->stick.center.x = joystick->pos.x + joystick->dir.x * dist;
    joystick->stick.center.y = joystick->pos.y + joystick->dir.y * dist;
}

void draw_joystick(Joystick* joystick)
{
    DrawCircleLinesV(joystick->pos, joystick->max_dist, RAYWHITE);
    DrawCircleV(joystick->stick.center, joystick->stick.radius, RAYWHITE);
}