#include <game_context.h>

typedef struct GameContext {
	float deltaTime;
	Rectangle screenBounds;
	float guiScale;
	Camera3D camera3D;
} GameContext;

GameContext gameContext = {0};

void InitializeGameContext()
{
	gameContext = (GameContext){
		.deltaTime = 0,

		.guiScale = 1,
		.screenBounds = {0, 0, GetRenderWidth(), GetRenderHeight()},
		.camera3D = (Camera3D){
			.position = (Vector3){-2, 3, 2},
			.target = (Vector3){0, 0, 0},
			.up = (Vector3){0, 1, 0},
			.fovy = 2,
			.projection = CAMERA_ORTHOGRAPHIC,
		},
	};
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

float GetDeltaTime() { return gameContext.deltaTime; }

Rectangle GetScreenBounds() { return gameContext.screenBounds; }

Camera3D GetCamera3D() { return gameContext.camera3D; }

float GetGuiScale() { return gameContext.guiScale; }

