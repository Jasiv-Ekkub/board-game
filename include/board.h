#ifndef BOARD_H
#define BOARD_H

#include <game_context.h>
#include <raylib.h>
#include <stdint.h>

#define FIELD_NAME_LENGTH 32

typedef struct Field
{
	char name[FIELD_NAME_LENGTH];
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
	int borderWidth;
	int dividerOffset;
	int fieldWidth;
	int cornerSize;

	Font font;
	float fontSize;
	float fontSpacing;

	int nameOffset;
	int commentOffset;

	struct
	{
		Color light;
		Color medium;
		Color dark;
	} colorPalette;

	Texture2D boardTexture;
} BoardGraphics;

typedef struct Board
{
	uint16_t fieldCount;
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
