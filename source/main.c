#include <board.h>
#include <assets.h>
#include <game_context.h>
#include <raygui.h>
#include <layout_engine.h>
#include <gui_elements.h>
#include <stdbool.h>

enum { MAIN_MENU, BOARD } phase = MAIN_MENU;
bool shouldClose = false;
Board board;

void HandleMainMenu();
void HandleBoard();

int main()
{
	InitializeGameContext();
	LoadAssets();

	board = LoadBoard();

	Color bgrColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
	while(!shouldClose)
	{
		shouldClose = WindowShouldClose();
		UpdateGameContext();
		if(IsKeyPressed(KEY_F4)) ToggleFullscreen();

		BeginDrawing();
			ClearBackground(bgrColor);
			switch(phase)
			{
				case MAIN_MENU:
					HandleMainMenu();
					break;
				case BOARD:
					HandleBoard();
					break;
			}
		EndDrawing();

	}

	UnloadBoard(board);	
	
	UnloadAssets();
	TerminateGameContext();
	return 0;
}

void HandleMainMenu()
{
	if(GuiButtonSfx(GetRectanglePlacement(0,0,500,100,CENTER,CENTER), "PLAY"))
	{
		SetupBoard(&board, 36, 1, 2);
		phase = BOARD;
	}
}

void HandleBoard()
{
	UpdateBoard(&board);
	if(HasGameEndedBoard(board))
	{
		phase = MAIN_MENU;
	}
}
