#include <board_logic.h>
#include <player_logic.h>
#include <layout_engine.h>

#include <stdio.h>

const char* boardPhaseNames[3] = {
	"Round beginning",
	"Rolling dice",
	"Round ending",
};

void HandleStartRound(Board* board, GameContext gameContext);
void HandleEndRound(Board* board, GameContext gameContext);

void UpdateBoardLogic(Board* board, GameContext gameContext)
{
	/*
	DrawRectangleRec(GetRectanglePlacement( 10, -10, 200, 200, LEFT, BOTTOM, gameContext), BLACK);
	DrawRectangleRec(GetRectanglePlacement(  0, -10, 200, 200, CENTER, BOTTOM, gameContext), BLACK);
	DrawRectangleRec(GetRectanglePlacement(-10, -10, 200, 200, RIGHT, BOTTOM, gameContext), BLACK);
	
	DrawRectangleRec(GetRectanglePlacement( 10,   0, 200, 200, LEFT, CENTER, gameContext), BLACK);
	DrawRectangleRec(GetRectanglePlacement(  0,   0, 200, 200, CENTER, CENTER, gameContext), BLACK);
	DrawRectangleRec(GetRectanglePlacement(-10,   0, 200, 200, RIGHT, CENTER, gameContext), BLACK);
	
	DrawRectangleRec(GetRectanglePlacement( 10,  10, 200, 200, LEFT, TOP, gameContext), BLACK);
	DrawRectangleRec(GetRectanglePlacement(  0,  10, 200, 200, CENTER, TOP, gameContext), BLACK);
	DrawRectangleRec(GetRectanglePlacement(-10,  10, 200, 200, RIGHT, TOP, gameContext), BLACK);
	*/

	DrawText(boardPhaseNames[board->phase], 10, 40, 20, LIME);
	char buffer[48];
	snprintf(buffer, 48, "Current player: %i", board->currentPlayer);
	DrawText(buffer, 10, 70, 20, LIME);
	snprintf(buffer, 48, "Board timer: %f", board->timer.currentTime);
	DrawText(buffer, 10, 100, 20, LIME);

	switch(board->phase)
	{
		case START_ROUND:
			HandleStartRound(board, gameContext);
			break;

		case ROLL_DICE:
			SetTimer(&board->timer, 5);
			if(UpdateTimer(&board->timer, gameContext) || GetPlayerResponse(board, gameContext))
			{
				ResetTimer(&board->timer);
				board->phase = END_ROUND;
			}
			break;

		case END_ROUND:
			HandleEndRound(board, gameContext);
			break;
	}
}

void HandleStartRound(Board* board, GameContext gameContext)
{
	board->currentPlayerResponse = NONE;
	board->phase = ROLL_DICE;
}

void HandleEndRound(Board* board, GameContext gameContext)
{
	board->currentPlayer++;
	board->currentPlayer %= board->playerCount;

	board->phase = START_ROUND;
}
