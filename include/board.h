#ifndef BOARD_H
#define BOARD_H

#include <raylib.h>

#include <game_context.h>
#include <board_assets.h>
#include <board_layout.h>
#include <field.h>
#include <player.h>
#include <timer.h>

#define MAX_FIELD_AMOUNT 64
#define MIN_FIELD_AMOUNT 24
#define MAX_PLAYER_AMOUNT 4

typedef struct Board
{
	BoardAssets assets;
	BoardLayout layout;

	int fieldCount;
	Field fields[MAX_FIELD_AMOUNT];

	int playerCount;
	Player players[MAX_PLAYER_AMOUNT];

	Vector3 position;

	Timer timer;
} Board;

Board LoadBoard();
void SetBoardSize(Board* board, int size);
void UpdateBoard(Board* board, GameContext gameContext);
void UnloadBoard(Board board);

#endif //BOARD_H
