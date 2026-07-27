#include <game_context.h>

void UpdateGameContext(GameContext* gameContext);
void UnloadGameContext(GameContext gameContext);

GameContext LoadGameContext()
{
	GameContext gameContext = {0};
	
	gameContext.camera3D = (Camera3D){
		.position = (Vector3){-2, 3, 2},
		.target = (Vector3){0, 0, 0},
		.up = (Vector3){0, 1, 0},
		.fovy = 2,
		.projection = CAMERA_ORTHOGRAPHIC,
	};

	gameContext.deltaTime = 0;

	gameContext.screenBounds.width = GetRenderWidth();
	gameContext.screenBounds.height = GetRenderHeight();

	return gameContext;
}

void UpdateGameContext(GameContext* gameContext)
{
	gameContext->deltaTime = GetFrameTime();

	if(IsWindowResized())
	{
		gameContext->screenBounds.width = GetRenderWidth();
		gameContext->screenBounds.height = GetRenderHeight();
	}
}

//Placeholder for potential changes
void UnloadGameContext(GameContext gameContext)
{

}
