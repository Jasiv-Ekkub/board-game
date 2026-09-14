#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <raylib.h>

void InitializeGameContext();
void UpdateGameContext();
void TerminateGameContext();

float GetDeltaTime();
Rectangle GetScreenBounds();
Camera3D GetCamera3D();
void SetCamera3DTarget(Vector3 target, float time);
Camera3D GetCameraDice();
float GetGuiScale();

#endif //GAME_CONTEXT_H
