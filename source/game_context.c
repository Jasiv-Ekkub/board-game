#define RAYGUI_IMPLEMENTATION
#define RAYGUI_MESSAGEBOX_BUTTON_HEIGHT 60
#include <raygui.h>
#include <style_buisness.h>
#include <game_context.h>
#include <stdlib.h>
#include <time.h>
#include <animator.h>

float deltaTime;
float timeSpeed;
Rectangle screenBounds;
float guiScale;
Camera3D camera3D;

void InitializeGameContext()
{
	InitWindow(1440, 810, "Board game");
	InitAudioDevice();
	GuiLoadStyleBuisness();
	SetExitKey(0);
	SetTargetFPS(60);

	srand(time(0));

	deltaTime = 0;
	timeSpeed = 1;
	guiScale = 1;
	screenBounds = (Rectangle){0, 0, GetRenderWidth(), GetRenderHeight()};
	camera3D = (Camera3D){
		.position = (Vector3){-2, 3, 2},
		.target = (Vector3){0, 0, 0},
		.up = (Vector3){0, 1, 0},
		.fovy = 2,
		.projection = CAMERA_ORTHOGRAPHIC,
	};
}

void UpdateGameContext()
{
	deltaTime = GetFrameTime();

	if(IsWindowResized())
	{
		screenBounds.width = GetRenderWidth();
		screenBounds.height = GetRenderHeight();
	}
}

void TerminateGameContext()
{
	CloseAudioDevice();
	CloseWindow();
}

float GetDeltaTime() { return deltaTime * timeSpeed; }

void SetTimeSpeed(float speed) { timeSpeed = speed; }

void SetCamera3DTarget(Vector3 target, float time)
{
	Animation animation = GetAnimation(&camera3D.target);
	AddKeyframe(&animation, time, target);
	QueueAnimation(animation);
}
void SetCameraFov(float fov)
{
	camera3D.fovy = fov;
}

Rectangle GetScreenBounds() { return screenBounds; }

Camera3D GetCamera3D() { return camera3D; }

float GetGuiScale() { return guiScale; }

void SetGuiScale(float scale) { guiScale = scale; }
