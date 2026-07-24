#include <board.h>

int main()
{
	InitWindow(1440, 810, "Board game");
	GameContext gameContext = LoadGameContext();
	Board board = LoadBoard("default", 12);

	Shader shader = LoadShader("resource/shaders/directional_light.vs", "resource/shaders/directional_light.fs");
	Model pawn = LoadModel("resource/models/pawn.glb");
	pawn.materials[0].shader = shader;
	Vector3 pawnPosition = {1, 0, 0};

	while(!WindowShouldClose())
	{
		UpdateGameContext(&gameContext);
		if(IsKeyPressed(KEY_F4)) ToggleFullscreen();

		pawnPosition.x += pawnPosition.z * gameContext.deltaTime;
		pawnPosition.z -= pawnPosition.x * gameContext.deltaTime;

		BeginDrawing();
			ClearBackground((Color){85, 85, 85, 255});
			DrawFPS(10, 10);

			UpdateBoard(&board, gameContext);

			BeginMode3D(gameContext.camera3D);

				DrawModel(pawn, pawnPosition, 0.25, RED);
				DrawGrid(10, 1);			
			EndMode3D();
		EndDrawing();
	}
	UnloadModel(pawn);


	UnloadBoard(board);	
	UnloadGameContext(gameContext);
	CloseWindow();
	return 0;
}
