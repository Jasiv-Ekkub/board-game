#include <game_context.h>

GameContext gameContext = {0};

void InitializeGameContext()
{
	gameContext.camera3D = (Camera3D){0};
	gameContext.camera3D.position = (Vector3){-10, 10, 10};
	gameContext.camera3D.target = (Vector3){0, 0, 0};
	gameContext.camera3D.up = (Vector3){0, 1, 0};
	gameContext.camera3D.fovy = 5;
	gameContext.camera3D.projection = CAMERA_ORTHOGRAPHIC;
}

void UpdateGameContext()
{
	gameContext.deltaTime = GetFrameTime();

	if(IsWindowResized())
	{
		gameContext.screenBounds.width = GetRenderWidth();
		gameContext.screenBounds.height = GetRenderHeight();
	}
}
