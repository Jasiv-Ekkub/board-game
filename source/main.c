#include <board.h>

int main()
{
	InitWindow(1440, 810, "Board game");
	GameContext gameContext = LoadGameContext();
	int tmp = 24;
	Board board = LoadBoard("default", tmp);

	while(!WindowShouldClose())
	{
		UpdateGameContext(&gameContext);
		if(IsKeyPressed(KEY_F4)) ToggleFullscreen();
		else if(IsKeyPressed(KEY_F5))
		{
			UnloadBoard(board);
			tmp += 4;
			board = LoadBoard("default", tmp);
		}
		
		BeginDrawing();
			ClearBackground((Color){85, 85, 85, 255});
			DrawFPS(10, 10);

			BeginMode3D(gameContext.camera3D);
				UpdateBoard(&board, gameContext);
			EndMode3D();
		EndDrawing();
	}

	UnloadBoard(board);	
	UnloadGameContext(gameContext);
	CloseWindow();
	return 0;
}
