#include <board.h>

int main()
{
	InitWindow(1440, 810, "Board game");
	GameContext gameContext = LoadGameContext();
	Board board = LoadBoard();
	int TMP = 24;
	SetBoardSize(&board, TMP);

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
			ClearBackground((Color){85, 85, 85, 255});
			DrawFPS(10, 10);

			UpdateBoard(&board, gameContext);
		EndDrawing();
	}

	UnloadBoard(board);	
	UnloadGameContext(gameContext);
	CloseWindow();
	return 0;
}
