#include <board.h>
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_MESSAGEBOX_BUTTON_HEIGHT 60
#include <raygui.h>
#include <style_buisness.h>
#include <game_context.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

bool shouldClose = false;

int main()
{
	InitWindow(1440, 810, "Board game");
	srand(time(0));
	SetExitKey(0);
	SetTargetFPS(60);
	GuiLoadStyleBuisness();

	InitializeGameContext();
	Board board = LoadBoard();
	
	Color bgrColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
	
	SetupBoard(&board, 36, 1, 2);

	while(!shouldClose)
	{
		shouldClose = WindowShouldClose();
		UpdateGameContext();
		if(IsKeyPressed(KEY_F4)) ToggleFullscreen();

		BeginDrawing();
			ClearBackground(bgrColor);
			UpdateBoard(&board);
		EndDrawing();

		shouldClose |= HasGameEndedBoard(board);
	}

	UnloadBoard(board);	
	CloseWindow();
	return 0;
}
