#include <game_context.h>
#include <draw_utilities.h>
#include <board_info.h>

int main()
{
	InitWindow(1440, 810, "Board game");

	Shader shader = LoadShader("resource/shaders/directional_light.vs", "resource/shaders/directional_light.fs");
	Model pawn = LoadModel("resource/models/pawn.glb");

	InitializeGameContext();
	LoadBoardInfo("resource/info/field_info.json");
	
	Texture2D boardTexture = LoadBoardTexture(6);
	Model boardModel = LoadBoardModel(boardTexture, 6);

	Rectangle tmpRect = {0,0,boardTexture.width,boardTexture.height};
	Rectangle tmpRect2 = {5,5,800,800};

	pawn.materials[0].shader = shader;

	while(!WindowShouldClose())
	{
		UpdateGameContext();

		if(IsKeyPressed(KEY_F4)) ToggleFullscreen();

		BeginDrawing();

		ClearBackground((Color){85, 55, 55, 255});

		BeginMode3D(gameContext.camera3D);

		DrawModel(pawn, (Vector3){0}, 0.25, RED);
		DrawModel(boardModel, (Vector3){0}, 1.0f, WHITE);
		//DrawGrid(10, 1);
		
		EndMode3D();
		
		//DrawTexturePro(boardTexture, tmpRect, tmpRect2, (Vector2){0}, 0, WHITE);

		EndDrawing();
	}
	
	UnloadModel(boardModel);
	UnloadTexture(boardTexture);

	UnloadModel(pawn);

	CloseWindow();

	return 0;
}
