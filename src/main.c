#include <raylib.h>
#include "include/game.h"

int main(int argc, char** argv)
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(FPS);

	init();

	while(!WindowShouldClose())
	{
		update();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		late_update();
		EndDrawing();
	}

	CloseWindow();
	finish();

	return 0;
}