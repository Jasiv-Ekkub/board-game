#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <raylib.h>

void InitializeGameContext();
void UpdateGameContext();

float GetDeltaTime();
Rectangle GetScreenBounds();
Camera3D GetCamera3D();
float GetGuiScale();

#endif //GAME_CONTEXT_H
