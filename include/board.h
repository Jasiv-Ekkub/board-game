#ifndef BOARD_H
#define BOARD_H

#include <game_context.h>
#include <raylib.h>
#include <stdint.h>

#define FIELD_NAME_LENGTH 32

typedef enum FieldType { PROPERTY, CHANCE } FieldType;

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
		};
		char comment[FIELD_NAME_LENGTH];
	};
	Color color;
} Field;

typedef struct Player
{
	char name[32];
	int16_t money;
} Player;

typedef struct BoardGraphics
{
	int borderWidth;
	int dividerOffset;
	int fieldWidth;
	int cornerSize;
} BoardGraphics;

typedef struct Board
{
	uint16_t fieldCount;
	Field fields[256];

	uint8_t playerCount;
	Player players[4];

	Texture2D boardTexture;
	Model boardModel;

	BoardGraphics graphics;
} Board;

void DebugPrintAllFields(Board board);

Board LoadBoard(const char* filename, uint16_t size);
void UpdateBoard(Board* board, GameContext gameContext);
void UnloadBoard(Board board);

#endif //BOARD_H
