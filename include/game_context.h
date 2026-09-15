#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <raylib.h>

void InitializeGameContext();
void UpdateGameContext();
void TerminateGameContext();

float GetDeltaTime();
void SetTimeSpeed(float speed);
Rectangle GetScreenBounds();
Camera3D GetCamera3D();
void SetCamera3DTarget(Vector3 target, float time);
void SetCameraFov(float fov);
float GetGuiScale();
void SetGuiScale(float scale);

#endif //GAME_CONTEXT_H
