#ifndef GAME_H
#define GAME_H

#define WIDTH 800
#define HEIGHT 600
#define TITLE "GEYM"
#define FPS 60
#define DEBUG 1
extern unsigned int ENEMY_COUNT;

void init();
void update();
void late_update();
void draw_ui();
void finish();

#endif
