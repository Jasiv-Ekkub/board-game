#include <game_context.h>

int main()
{
	InitWindow(1440, 810, "Board game");

	Shader shader = LoadShader("resource/shaders/directional_light.vs", "resource/shaders/directional_light.fs");
	Model cube = LoadModel("resource/models/pawn.glb");
	
	InitializeGameContext();

	cube.materials[0].shader = shader;

	while(!WindowShouldClose())
	{
		UpdateGameContext();

		BeginDrawing();

		BeginMode3D(gameContext.camera3D);
		BeginShaderMode(shader);

		for(float i = -2; i <= 2; ++i)
		for(float j = -2; j <= 2; ++j)
		DrawModel(cube, (Vector3){i,0,j}, 0.25, RED);
		DrawGrid(10, 1);
		
		EndShaderMode();
		EndMode3D();

		EndDrawing();
	}

	UnloadModel(cube);

	CloseWindow();

	return 0;
}
