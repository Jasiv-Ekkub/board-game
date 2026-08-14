#ifndef BOARD_H
#define BOARD_H

#include <raylib.h>

#include <game_context.h>
#include <board_assets.h>
#include <board_layout.h>
#include <field.h>
#include <player.h>
#include <timer.h>

#define MAX_FIELD_AMOUNT 48
#define MIN_FIELD_AMOUNT 24
#define MAX_PLAYER_AMOUNT 4

typedef enum BoardPhase
{
	START_ROUND = 0,
	ROLL_DICE,
	CHECK_FIELD,
	PAY_FEE,
	BUY_FIELD,
	UPGRADE_BUILDING,
	CHECK_DEBT,
	END_ROUND,
} BoardPhase;

extern const char* boardPhaseNames[];

typedef struct Board
{
	BoardAssets assets;
	BoardLayout layout;
	
	BoardPhase phase;

	int fieldCount;
	Field fields[MAX_FIELD_AMOUNT];

	int playerCount;
	Player players[MAX_PLAYER_AMOUNT];
	int currentPlayer;
	PlayerResponse currentPlayerResponse;
	int currentDiceroll;

	Vector3 position;

	Timer timer;
	Timer gameTimer;
} Board;

Board LoadBoard();
void SetupBoard(Board* board, int size, int humanCount, int botCount);
void UpdateBoard(Board* board, GameContext gameContext);
void UnloadBoard(Board board);
bool HasGameEnded(Board board);
Player GetWinner(Board board);

#endif //BOARD_H
