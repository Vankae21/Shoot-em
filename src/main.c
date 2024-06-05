#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include "include/game.h"
#include "include/vutils.h"

int main(int argc, char** argv)
{
	
	for(int i = 0; i < argc; i++)
	{
		if (strcmp("--no-shadow", argv[i]) == 0) {
			IS_SHADOWED = false;
		} else if (strcmp("-f", argv[i]) == 0) {
			IS_FULLSCREEN = true;
			WIDTH = 0;
			HEIGHT = 0;
		}

	}
	InitWindow(WIDTH, HEIGHT, TITLE);
	if (IS_FULLSCREEN) {
		ToggleFullscreen();
		WIDTH = GetScreenWidth(), HEIGHT = GetScreenHeight();
	}
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
