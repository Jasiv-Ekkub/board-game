#include <board_logic.h>
#include <player_logic.h>
#include <layout_engine.h>

#include <stdio.h>
#include <stdlib.h>

#include <gui_elements.h>

#define BUFFER_SIZE 16

void HandleStartRound(Board* board, GameContext gameContext);
void HandleRollDice(Board* board, GameContext gameContext);
void HandleCheckField(Board* board, GameContext gameContext);
void HandlePayFee(Board* board, GameContext gameContext);
void HandleBuyField(Board* board, GameContext gameContext);
void HandleUpgradeBuilding(Board* board, GameContext gameContext);
void HandleCheckDebt(Board* board, GameContext gameContext);
void HandleEndRound(Board* board, GameContext gameContext);

void UpdateBoardLogic(Board* board, GameContext gameContext)
{
	UpdateTimer(&board->gameTimer, gameContext);
	char buffer[BUFFER_SIZE];
	GuiPlayerInfo(GetRectanglePlacement( 10,  10, 225, 100, LEFT, TOP, gameContext), board->players[0]);
	GuiPlayerInfo(GetRectanglePlacement(-10,  10, 225, 100, RIGHT, TOP, gameContext), board->players[1]);
	GuiPlayerInfo(GetRectanglePlacement( 10, -10, 225, 100, LEFT, BOTTOM, gameContext), board->players[2]);
	GuiPlayerInfo(GetRectanglePlacement(-10, -10, 225, 100, RIGHT, BOTTOM, gameContext), board->players[3]);
	
	int gameTime = board->gameTimer.currentTime;
	snprintf(buffer, BUFFER_SIZE, "%02i:%02i", gameTime/60, gameTime%60);
	GuiBoxText(GetRectanglePlacement(-240, 10, 240, 60, CENTER, TOP, gameContext), buffer);
	GuiBoxText(GetRectanglePlacement(240, 10, 240, 60, CENTER, TOP, gameContext), boardPhaseNames[board->phase]);

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

		case BUY_FIELD:
			HandleBuyField(board, gameContext);
			break;

		case UPGRADE_BUILDING:
			HandleUpgradeBuilding(board, gameContext);
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
		for(int i=1; i<=currentDiceroll; ++i)
		{
			int position = (player->position + i) % board->fieldCount;
			Field field = board->fields[position];
			if(field.type == SUPERACTION)
			{
				field.action(player);
			}
		}
		player->position += currentDiceroll;
		player->position %= board->fieldCount;
		
		if(player->money < 0) board->phase = CHECK_DEBT;
		else board->phase = CHECK_FIELD;
	}
}

void HandleCheckField(Board* board, GameContext gameContext)
{
	Player* player = &board->players[board->currentPlayer];
	Field* field = &board->fields[player->position];
	PrintField(*field);
	switch(field->type)
	{
		case PROPERTY:
			if(field->ownerId == -1)
			{
				if(GetFieldValue(*field) <= player->money)
				{
					board->phase = BUY_FIELD;
					return;
				}
			}
			else if(field->ownerId == board->currentPlayer)
			{
				if(field->buildingLevel < MAX_BUILDING_LEVEL - 1 && GetUpgradeValue(*field) <= player->money)
				{
					board->phase = UPGRADE_BUILDING;	
					return;
				}
			}
			else
			{
				board->phase = PAY_FEE;
				return;
			}
			break;
		
		case ACTION:
			field->action(player);
			board->phase = CHECK_DEBT;
			return;

		default:
			break;
	}
	board->phase = END_ROUND;
}

void HandlePayFee(Board* board, GameContext gameContext)
{
	Player* player = &board->players[board->currentPlayer];
	Field* field = &board->fields[player->position];
	Player* owner = &board->players[field->ownerId];

	int fee = GetFeeValue(*field);
	player->money -= fee;
	owner->money += fee;

	if(GetFieldValue(*field) <= player->money)
		board->phase = BUY_FIELD;

	else if(player->money < 0)
		board->phase = CHECK_DEBT;
	
	else board->phase = END_ROUND;
}

void HandleBuyField(Board* board, GameContext gameContext)
{
	SetTimer(&board->timer, 30);
	if(UpdateTimer(&board->timer, gameContext) || GetPlayerResponse(board, gameContext))
	{
		ResetTimer(&board->timer);
		if(board->currentPlayerResponse == POSITIVE)
		{
			Player* player = &board->players[board->currentPlayer];
			Field* field = &board->fields[player->position];
			
			int cost = GetFieldValue(*field);
			if(field->ownerId != -1)
			{
				Player* owner = &board->players[field->ownerId];
				owner->money += cost;
			}
			player->money -= cost;
			field->ownerId = board->currentPlayer;

		}
		board->phase = END_ROUND;
	}
}

void HandleUpgradeBuilding(Board* board, GameContext gameContext)
{
	SetTimer(&board->timer, 5);
	if(UpdateTimer(&board->timer, gameContext) || GetPlayerResponse(board, gameContext))
	{
		ResetTimer(&board->timer);
		if(board->currentPlayerResponse == POSITIVE)
		{
			Player* player = &board->players[board->currentPlayer];
			Field* field = &board->fields[player->position];
			
			field->buildingLevel++;
			player->money -= GetUpgradeValue(*field);
		}
		board->phase = END_ROUND;
	}
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

	return;

	printf("\nBoard status\n");
	for(int i=0; i<board->fieldCount; ++i)
	{
		PrintField(board->fields[i]);
	}
}
