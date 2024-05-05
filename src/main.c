#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include "include/game.h"
#include "include/vutils.h"

int main(int argc, char** argv)
{
	if(argc > 1)
		IS_SHADOWED = !strcmp("--shadow", argv[1]);
	if(argc > 2)
		ENEMY_COUNT = str_to_int(argv[2]);
	if(argc > 3)
		WEAPON_COUNT = str_to_int(argv[3]);

	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(FPS);

	init();
	while(!WindowShouldClose())
	{
		update();
		BeginDrawing();
		ClearBackground((Color){100, 100, 100, 255});
		late_update();
		draw_ui();
		EndDrawing();
	}

	CloseWindow();
	finish();

	return 0;
}
