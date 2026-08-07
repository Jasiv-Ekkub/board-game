#include <board.h>
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_MESSAGEBOX_BUTTON_HEIGHT 60
#include <raygui.h>
#include <style_buisness.h>

int main()
{
	InitWindow(1440, 810, "Board game");
	GuiLoadStyleBuisness();

	GameContext gameContext = LoadGameContext();
	Board board = LoadBoard();
	int TMP = 24;
	SetBoardSize(&board, TMP);

	Color bgrColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));

	while(!WindowShouldClose())
	{
		UpdateGameContext(&gameContext);
		if(IsKeyPressed(KEY_F4)) ToggleFullscreen();
		else if(IsKeyPressed(KEY_F5))
		{
			TMP += 4;
			SetBoardSize(&board, TMP);
		}
		else if(IsKeyPressed(KEY_F6)) gameContext.guiScale += 0.05f;
		else if(IsKeyPressed(KEY_F7)) gameContext.guiScale -= 0.05f;
		BeginDrawing();
			ClearBackground(bgrColor);
			UpdateBoard(&board, gameContext);
		EndDrawing();
	}

	UnloadBoard(board);	
	UnloadGameContext(gameContext);
	CloseWindow();
	return 0;
}
