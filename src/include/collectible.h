#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <raylib.h>
#include "vutils.h"

typedef struct
{
    enum
    {
        COIN,
        MEDKIT
    } type;
    Circle cir;
    bool is_collected;
    float offset;
    float passed_time;
    float drop_time;
    float velocity;
    bool is_floating;
    float dir_x;
} Collectible;

Collectible* init_collectible(unsigned int type, Vector2 pos);
void draw_collectible(Collectible* collectible, Texture2D collectible_spsheet);
void rerow_collectibles(Collectible*** list, unsigned int* count);
void add_collectible(Collectible* c, Collectible*** list, unsigned int* count);

#endif