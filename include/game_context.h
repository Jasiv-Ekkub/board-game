#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <raylib.h>

typedef struct GameContext {
	float deltaTime;
	Rectangle screenBounds;
	Camera3D camera3D;
} GameContext;

GameContext LoadGameContext();
void UpdateGameContext(GameContext* gameContext);
void UnloadGameContext(GameContext gameContext);

#endif //GAME_CONTEXT_H
