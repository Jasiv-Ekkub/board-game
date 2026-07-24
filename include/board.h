#ifndef BOARD_H
#define BOARD_H

#include <game_context.h>
#include <raylib.h>
#include <stdint.h>

typedef struct Field
{
	char name[32];
	int16_t value;
	uint8_t buildingLevel;
	uint8_t ownerId;
	Color color;
} Field;

typedef struct Player
{
	char name[32];
	int16_t money;
} Player;

typedef struct BoardGraphics
{
	int fieldBorder;
	int fieldDivider;
	int fieldWidth;
	int fieldHeight;

	float fontSize;
	float fontSpacing;

	int nameOffset;
	int commentOffset;

	Color boardLight;
	Color boardDark;

	Texture2D boardTexture;
} BoardGraphics;

typedef struct Board
{
	uint16_t size;
	Field fields[256];

	uint8_t playerCount;
	Player players[4];

	BoardGraphics graphics;
} Board;

void DebugPrintAllFields(Board board);

Board LoadBoard(const char* filename, uint16_t size);
void UpdateBoard(Board* board, GameContext gameContext);
void UnloadBoard(Board board);

#endif //BOARD_H
