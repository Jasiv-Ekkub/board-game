#include <game_context.h>
#include <draw_utilities.h>

int main()
{
	InitWindow(1440, 810, "Board game");

	Shader shader = LoadShader("resource/shaders/directional_light.vs", "resource/shaders/directional_light.fs");
	Model pawn = LoadModel("resource/models/pawn.glb");

	InitializeGameContext();

	Texture2D boardTexture = LoadBoardTexture(6);
	Rectangle tmpRect = {0,0,boardTexture.width,boardTexture.height};
	Rectangle tmpRect2 = {5,5,800,800};

	pawn.materials[0].shader = shader;

	while(!WindowShouldClose())
	{
		UpdateGameContext();

		BeginDrawing();

		ClearBackground((Color){85, 55, 55, 255});

		BeginMode3D(gameContext.camera3D);

		for(float i = -2; i <= 2; ++i)
		for(float j = -2; j <= 2; ++j)
		DrawModel(pawn, (Vector3){i,0,j}, 0.25, RED);
		DrawGrid(10, 1);
		
		EndMode3D();
		
		DrawTexturePro(boardTexture, tmpRect, tmpRect2, (Vector2){0}, 0, WHITE);

		EndDrawing();
	}
	
	UnloadTexture(boardTexture);

	UnloadModel(pawn);

	CloseWindow();

	return 0;
}
