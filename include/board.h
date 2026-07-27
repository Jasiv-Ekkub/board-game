#ifndef BOARD_H
#define BOARD_H

#include <game_context.h>
#include <raylib.h>
#include <stdint.h>

#define FIELD_NAME_LENGTH 32

typedef enum FieldType { PROPERTY, ACTION } FieldType;

typedef struct Field
{
	char name[FIELD_NAME_LENGTH];
	FieldType type;
	union
	{
		struct {
			int16_t value;
			uint8_t buildingLevel;
			uint8_t ownerId;
			Color color;
		};
		struct {
			char comment[FIELD_NAME_LENGTH];
			uint16_t imageId;
		};
	};
} Field;

typedef struct Player
{
	char name[32];
	uint16_t position;
	Color color;
} Player;

typedef struct BoardGraphics
{
	uint64_t borderWidth;
	uint64_t dividerOffset;
	uint64_t fieldWidth;
	uint64_t cornerSize;

	uint64_t pixelSize;
} BoardGraphics;

typedef struct Board
{
	uint16_t fieldCount;
	Field fields[256];

	uint8_t playerCount;
	Player players[4];

	Texture2D boardTexture;
	Model boardModel;

	Shader pawnShader;
	Model pawnModel;

	BoardGraphics graphics;

	float FOOBAR;
} Board;

void DebugPrintAllFields(Board board);

Board LoadBoard(const char* filename, uint16_t size);
void UpdateBoard(Board* board, GameContext gameContext);
void UnloadBoard(Board board);

#endif //BOARD_H
