#include <board_logic.h>
#include <player_logic.h>
#include <layout_engine.h>

#include <stdio.h>
#include <stdlib.h>

const char* boardPhaseNames[3] = {
	"Round beginning",
	"Rolling dice",
	"Round ending",
};

void HandleStartRound(Board* board, GameContext gameContext);
void HandleRollDice(Board* board, GameContext gameContext);
void HandleCheckField(Board* board, GameContext gameContext);
void HandlePayFee(Board* board, GameContext gameContext);
void HandleCheckDebt(Board* board, GameContext gameContext);
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
			HandleRollDice(board, gameContext);
			break;

		case CHECK_FIELD:
			HandleCheckField(board, gameContext);
			break;
	
		case PAY_FEE:
			HandlePayFee(board, gameContext);
			break;

		case CHECK_DEBT:
			HandleCheckDebt(board, gameContext);
			break;

		case END_ROUND:
			HandleEndRound(board, gameContext);
			break;
		default:
			board->phase = END_ROUND;
			break;
	}
}

void HandleStartRound(Board* board, GameContext gameContext)
{
	board->currentPlayerResponse = NONE;
	Player* player = &board->players[board->currentPlayer];
	if(player->money < 0)
	{
		board->phase = END_ROUND;
		return;
	}

	board->phase = ROLL_DICE;
}

void HandleRollDice(Board* board, GameContext gameContext)
{
	SetTimer(&board->timer, 5);
	if(UpdateTimer(&board->timer, gameContext) || GetPlayerResponse(board, gameContext))
	{
		ResetTimer(&board->timer);

		int currentDiceroll = 1 + rand()%6;
		board->currentDiceroll = currentDiceroll;

		Player* player = &board->players[board->currentPlayer];
		player->position += currentDiceroll;
		player->position %= board->fieldCount;

		board->phase = CHECK_FIELD;
	}
}

void HandleCheckField(Board* board, GameContext gameContext)
{
	Player* player = &board->players[board->currentPlayer];
	Field* field = &board->fields[player->position];
	switch(field->type)
	{
		case PROPERTY:
			if(field->ownerId == -1)
				board->phase = BUY_FIELD;
			else if(field->ownerId == board->currentPlayer)
				board->phase = UPGRADE_BUILDING;	
			else board->phase = PAY_FEE;
			break;
		
		case ACTION:
			field->action(player);
			board->phase = CHECK_DEBT;
			break;

		default:
			board->phase = END_ROUND;
			break;
	}
}

void HandlePayFee(Board* board, GameContext gameContext)
{
	Player* player = &board->players[board->currentPlayer];
	Field* field = &board->fields[player->position];
	Player* fieldOwner = &board->players[field->ownerId];

	int fee = GetFeeValue(*field);
	fieldOwner->money += fee;
	player->money -= fee;
	
	if(player->money > 0) board->phase = BUY_FIELD;
	else board->phase = CHECK_DEBT;
}

void HandleCheckDebt(Board* board, GameContext gameContext)
{
	Player* player = &board->players[board->currentPlayer];
	board->phase = END_ROUND;

	for(int i=0; i<board->fieldCount; ++i)
	{
		if(player->money >= 0) break;

		Field* field = &board->fields[i];
		if(field->type != PROPERTY || field->ownerId != board->currentPlayer) continue;

		field->ownerId = -1;
		player->money += field->value;
	}
}

void HandleEndRound(Board* board, GameContext gameContext)
{
	board->currentPlayer++;
	board->currentPlayer %= board->playerCount;

	board->phase = START_ROUND;
}
