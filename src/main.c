#include <raylib.h>
#include "include/game.h"

int main(int argc, char** argv)
{
	if(argc > 1)
		ENEMY_COUNT = argv[1][0] - 48;

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
