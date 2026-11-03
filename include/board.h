#ifndef BOARD_H
#define BOARD_H

#include <raylib.h>

#include <board_layout.h>
#include <field.h>
#include <player.h>
#include <timer.h>

#define MAX_FIELD_COUNT 40
#define MIN_FIELD_COUNT 24 
#define MAX_PLAYER_COUNT 4
#define MAX_POPUP_COUNT 16
#define MAX_POPUP_LENGTH 128
#define MAX_START_MONEY 10000
#define MIN_START_MONEY 1000
#define START_MONEY_STEP 500

typedef enum BoardPhase
{
	START_ROUND = 0,
	ROLL_DICE,
	MOVE_PLAYER,
	CHECK_FIELD,
	PAY_FEE,
	BUY_FIELD,
	UPGRADE_BUILDING,
	CHECK_DEBT,
	CHECK_WIN,
	END_ROUND,

	SHOW_WINNER,
	END_GAME,
} BoardPhase;

typedef struct Board
{
	BoardLayout layout;
	
	BoardPhase phase;

	int fieldCount;
	Field fields[MAX_FIELD_COUNT];

	int playerCount;
	Player players[MAX_PLAYER_COUNT];
	int currentPlayer;
	int winnerId;
	bool forceEnd;

	PlayerResponse currentPlayerResponse;
	int currentDiceroll;

	int popupCount;
	char popups[MAX_POPUP_COUNT][MAX_POPUP_LENGTH];

	Vector3 position;

	float diceAngle;
	Vector3 diceAxis;
	
	struct {
		Color light;
		Color dark;
		Color text;
	} colors;

	bool hasGameEnded;

	Timer delayTimer;
	Timer popupTimer;
} Board;

extern Board board;

void InitializeBoard();
void SetupBoard(int size, int humanCount, int botCount, int startMoney);
void UpdateBoard();
bool HasGameEndedBoard();
void AddPopupBoard(const char* popup);

#endif //BOARD_H
