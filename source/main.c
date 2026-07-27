#include <board.h>

int main()
{
	InitWindow(1440, 810, "Board game");
	GameContext gameContext = LoadGameContext();
	int tmp = 16;
	Board board = LoadBoard("default", tmp);

	Shader shader = LoadShader("resource/shaders/directional_light.vs", "resource/shaders/directional_light.fs");
	Model pawn = LoadModel("resource/models/pawn.glb");
	pawn.materials[0].shader = shader;
	Vector3 pawnPosition = {1, 0, 0};

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

		pawnPosition.x += pawnPosition.z * gameContext.deltaTime;
		pawnPosition.z -= pawnPosition.x * gameContext.deltaTime;

		BeginDrawing();
			ClearBackground((Color){85, 85, 85, 255});
			DrawFPS(10, 10);


			BeginMode3D(gameContext.camera3D);

				DrawModel(pawn, pawnPosition, 0.0625, RED);
				DrawGrid(10, 1);			
				UpdateBoard(&board, gameContext);
			EndMode3D();
		EndDrawing();
	}
	UnloadModel(pawn);


	UnloadBoard(board);	
	UnloadGameContext(gameContext);
	CloseWindow();
	return 0;
}
