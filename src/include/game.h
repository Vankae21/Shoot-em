#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include "gamecamera.h"

#define TITLE "Shoot Those Mfs"
#define FPS 60
#define SIZE_MULTIPLIER ((float)WIDTH / (float)1280)
#define DEBUG 0
#define DEF_WIDTH 1280
#define DEF_HEIGHT 720
#define SHADOW_OFFSET 8
#define SHADOW_COLOR (Color){ 0, 0, 0, 50 }
extern bool IS_FULLSCREEN, IS_SHADOWED;
extern unsigned int ENEMY_COUNT, WEAPON_COUNT;
extern GameCamera* gamecamera;
extern int WIDTH, HEIGHT;


void init();
void update();
void late_update();
void draw_ui();
void finish();

#endif