#ifndef BOARD_H
#define BOARD_H

#include <raylib.h>

#include <board_layout.h>
#include <field.h>
#include <player.h>
#include <timer.h>

#define MAX_FIELD_COUNT 48
#define MIN_FIELD_COUNT 24
#define MAX_PLAYER_COUNT 4
#define MAX_POPUP_COUNT 16

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
	END_GAME,
} BoardPhase;

extern const char* boardPhaseNames[];

typedef struct Board
{
	BoardLayout layout;
	
	BoardPhase phase;

	int fieldCount;
	Field fields[MAX_FIELD_COUNT];

	int playerCount;
	Player players[MAX_PLAYER_COUNT];
	int currentPlayer;
	PlayerResponse currentPlayerResponse;
	int currentDiceroll;

	int popupCount;
	const char* popups[MAX_POPUP_COUNT];

	Vector3 position;
	
	struct {
		Color light;
		Color dark;
	} colors;

	bool hasGameEnded;

	Timer timer;
	Timer gameTimer;
	Timer popupTimer;
} Board;

Board LoadBoard();
void SetupBoard(Board* board, int size, int humanCount, int botCount);
void UpdateBoard(Board* board);
void UnloadBoard(Board board);
bool HasGameEndedBoard(Board board);
void AddPopupBoard(Board* board, const char* popup);

#endif //BOARD_H
