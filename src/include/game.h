#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include "gamecamera.h"

#define WIDTH 1280
#define HEIGHT 720
#define TITLE "GEYM"
#define FPS 60
#define DEBUG 0
#define SIZE_MULTIPLIER 2
extern unsigned int ENEMY_COUNT, WEAPON_COUNT;
extern GameCamera* gamecamera;

void init();
void update();
void late_update();
void draw_ui();
void finish();

#endif