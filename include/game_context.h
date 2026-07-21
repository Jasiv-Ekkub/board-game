#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <raylib.h>

typedef struct GameContext {
	float deltaTime;
	Rectangle screenBounds;
	Camera3D camera3D;
} GameContext;

extern GameContext gameContext;

void InitializeGameContext();
void UpdateGameContext();

#endif //GAME_CONTEXT_H
