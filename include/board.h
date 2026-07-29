#ifndef BOARD_H
#define BOARD_H

#include <model_defines.h>
#include <game_context.h>
#include <raylib.h>

#define FIELD_NAME_LENGTH 32
#define MAX_BUILDING_LEVEL 3

typedef enum FieldType { PROPERTY, ACTION } FieldType;

typedef struct Field
{
	char name[FIELD_NAME_LENGTH];
	FieldType type;
	union
	{
		struct {
			int value;
			int buildingLevel;
			int ownerId;
			Color color;
		};
		struct {
			char comment[FIELD_NAME_LENGTH];
			int imageId;
		};
	};
} Field;

typedef struct Player
{
	char name[32];
	int position;
	Color color;
} Player;

typedef struct BoardGraphics
{
	float borderWidth;
	float dividerOffset;
	float fieldWidth;
	float cornerSize;

	float pawnOffset;
	float modelScale16;
	float modelScale64;

	float pixelSize;
} BoardGraphics;

typedef struct Board
{
	int fieldCount;
	Field fields[256];

	int playerCount;
	Player players[4];

	Texture2D boardTexture;
	Model boardModel;

	Shader lightShader;
	Model models[MODEL_COUNT];
	int buildingModelIds[MAX_BUILDING_LEVEL];

	Vector3 position;
	BoardGraphics graphics;
} Board;

void DebugPrintAllFields(Board board);

Board LoadBoard(const char* filename, int size);
void UpdateBoard(Board* board, GameContext gameContext);
void UnloadBoard(Board board);

#endif //BOARD_H
