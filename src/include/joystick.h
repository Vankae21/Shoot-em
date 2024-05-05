#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <raylib.h>
#include <math.h>
#include "vutils.h"

typedef struct
{
    Vector2 pos;
    float max_dist;
    Circle stick;
    Vector2 dir;
} Joystick;

Joystick* init_joystick(Vector2 pos, float max_dist, float radius);
void update_joystick(Joystick* joystick);
void draw_joystick(Joystick* joystick);

#endif