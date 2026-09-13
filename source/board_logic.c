#include <board_logic.h>
#include <player_logic.h>
#include <layout_engine.h>
#include <animator.h>
#include <helpers.h>

#include <stdio.h>
#include <stdlib.h>

#include <raygui.h>
#include <gui_elements.h>

#define BUFFER_SIZE 64

void HandleStartRound(Board* board);
void HandleRollDice(Board* board);
void HandleMovePlayer(Board* board);
void HandleCheckField(Board* board);
void HandlePayFee(Board* board);
void HandleBuyField(Board* board);
void HandleUpgradeBuilding(Board* board);
void HandleCheckDebt(Board* board);
void HandleEndRound(Board* board);
void HandleEndGame(Board* board);

void UpdateBoardLogic(Board* board)
{	
	Timer_Update(&board->gameTimer);
	Timer_Update(&board->delayTimer);
	board->diceAngle += 90 * GetDeltaTime();
	if(board->phase != END_GAME)
	{
		if(Timer_HasEnded(board->gameTimer) || GuiButtonSfx(GetRectanglePlacement(10, 0, 100, 60, LEFT, CENTER), "Exit"))
		{
			Timer_Set(&board->gameTimer, 0);
			Timer_Set(&board->delayTimer, 0);
			board->phase = END_ROUND;
			board->currentDiceroll = 0;
			board->popupCount = 0;
			board->playedPopupSound = 0;
		}

		char buffer[BUFFER_SIZE];
		int gameTime = board->gameTimer.time;
		snprintf(buffer, BUFFER_SIZE, "%02i:%02i", gameTime/60, gameTime%60);
		
		GuiBoxText(GetRectanglePlacement(260, 10, 225, 60, CENTER, TOP), boardPhaseNames[board->phase]);
		GuiBoxText(GetRectanglePlacement(260, 70, 225, 60, CENTER, TOP), buffer);
		GuiPlayerInfo(GetRectanglePlacement(-260, 10, 225, 120, CENTER, TOP), board->players[board->currentPlayer]);

		GuiPlayerInfo(GetRectanglePlacement( 10,  10, 225, 120, LEFT, TOP), board->players[0]);
		GuiPlayerInfo(GetRectanglePlacement(-10,  10, 225, 120, RIGHT, TOP), board->players[1]);
		GuiPlayerInfo(GetRectanglePlacement( 10, -10, 225, 120, LEFT, BOTTOM), board->players[2]);
		GuiPlayerInfo(GetRectanglePlacement(-10, -10, 225, 120, RIGHT, BOTTOM), board->players[3]);
	}
	else
	{
		GuiGameOver(GetRectanglePlacement(0, 0, 500, 200, CENTER, CENTER), board->players[0]);
	}

	if(board->popupCount > 0 || !Timer_HasEnded(board->delayTimer)) return;

	switch(board->phase)
	{
		case START_ROUND:
			HandleStartRound(board);
			break;
		case ROLL_DICE:
			HandleRollDice(board);
			break;
		case MOVE_PLAYER:
			HandleMovePlayer(board);
			break;
		case CHECK_FIELD:
			HandleCheckField(board);
			break;
		case BUY_FIELD:
			HandleBuyField(board);
			break;
		case UPGRADE_BUILDING:
			HandleUpgradeBuilding(board);
			break;
		case PAY_FEE:
			HandlePayFee(board);
			break;
		case CHECK_DEBT:
			HandleCheckDebt(board);
			break;
		case END_ROUND:
			HandleEndRound(board);
			break;
		case END_GAME:
			HandleEndGame(board);
			break;
		default:
			board->phase = END_ROUND;
			break;
	}
}

void HandleStartRound(Board* board)
{
	Player* player = &board->players[board->currentPlayer];
	bool skip = false;
	if(player->turnSkips > 0)
	{
		char buffer[MAX_POPUP_LENGTH] = {0};
		snprintf(buffer, MAX_POPUP_LENGTH, "Player skips %i round(s)", player->turnSkips--);
		AddPopupBoard(board, buffer);
		skip = true;
	}

	skip |= player->money < 0;
	if(skip)
	{
		board->phase = END_ROUND;
		return;
	}
	board->diceAxis = (Vector3){
		GetRandomFloat(-1, 1),
		GetRandomFloat(-1, 1),
		GetRandomFloat(-1, 1)
	};
	board->currentDiceroll = -1;
	board->phase = ROLL_DICE;
	Timer_Set(&board->delayTimer, 0.5);
}

void HandleRollDice(Board* board)
{
	if(GetPlayerResponse(board))
	{
		board->currentDiceroll = 1 + rand()%6;
		board->phase = MOVE_PLAYER;
		PlaySound(sounds[DICE_HIT_SOUND]);
		Timer_Set(&board->delayTimer, 1);
	}
}

void HandleMovePlayer(Board* board)
{
	Player* player = &board->players[board->currentPlayer];
	for(int i=1; i<=board->currentDiceroll; ++i)
	{
		int position = (player->position + i) % board->fieldCount;
		Field field = board->fields[position];
		if(field.type == SUPERACTION)
		{
			field.action(board);
		}
	}
	int oldPosition = player->position;
	player->position += board->currentDiceroll;
	player->position %= board->fieldCount;

	int quarter = board->fieldCount/4;
	int ppd = player->position/quarter;
	Animation animation = GetAnimation(&board->players[board->currentPlayer].modelPosition);
	if(oldPosition/quarter != ppd)
	{
		AddKeyframe(&animation, 0.5, CalculatePlayerPosition(*board, board->currentPlayer, ppd * quarter));
	}
	AddKeyframe(&animation, 1, CalculatePlayerPosition(*board, board->currentPlayer, player->position));
	QueueAnimation(animation);
	
	board->currentDiceroll = 0;
	board->phase = CHECK_FIELD;
}

void HandleCheckField(Board* board)
{
	Player* player = &board->players[board->currentPlayer];
	Field* field = &board->fields[player->position];
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
			field->action(board);
			break;
		default:
			break;
	}
	board->phase = CHECK_DEBT;
}

void HandlePayFee(Board* board)
{
	Player* player = &board->players[board->currentPlayer];
	Field* field = &board->fields[player->position];
	Player* owner = &board->players[field->ownerId];

	int fee = GetFeeValue(*field);

	char buffer[MAX_POPUP_LENGTH] = {0};
	snprintf(buffer, MAX_POPUP_LENGTH, "The amount due is $%i", fee);
	AddPopupBoard(board, buffer);
	player->money -= fee;
	owner->money += fee;

	if(GetFieldValue(*field) <= player->money) board->phase = BUY_FIELD;
	else board->phase = CHECK_DEBT;

	PlaySound(sounds[KA_CHING_SOUND]);
}

void HandleBuyField(Board* board)
{
	if(GetPlayerResponse(board))
	{
		if(board->currentPlayerResponse == POSITIVE)
		{
			Player* player = &board->players[board->currentPlayer];
			Field* field = &board->fields[player->position];
			
			int cost = GetFieldValue(*field);
			char buffer[MAX_POPUP_LENGTH] = {0};
			snprintf(buffer, MAX_POPUP_LENGTH, "%s was bought for $%i", field->name, cost);
			AddPopupBoard(board, buffer);

			if(field->ownerId != -1)
			{
				Player* owner = &board->players[field->ownerId];
				owner->money += cost;
			}
			player->money -= cost;
			field->ownerId = board->currentPlayer;
			PlaySound(sounds[KA_CHING_SOUND]);
		}
		board->phase = CHECK_DEBT;
	}
}

void HandleUpgradeBuilding(Board* board)
{
	if(GetPlayerResponse(board))
	{
		if(board->currentPlayerResponse == POSITIVE)
		{
			Player* player = &board->players[board->currentPlayer];
			Field* field = &board->fields[player->position];

			int cost = GetUpgradeValue(*field);
			char buffer[MAX_POPUP_LENGTH] = {0};
			snprintf(buffer, MAX_POPUP_LENGTH, "%s was developed for $%i", field->name, cost);
			AddPopupBoard(board, buffer);
			
			player->money -= cost;
			field->buildingLevel++;
			PlaySound(sounds[KA_CHING_SOUND]);
		}
		board->phase = CHECK_DEBT;
	}
}

void HandleCheckDebt(Board* board)
{
	Player* player = &board->players[board->currentPlayer];

	int count = 0;
	int sum_cost = 0;

	for(int i=0; i<board->fieldCount; ++i)
	{
		if(player->money >= 0) break;

		Field* cheapestOwnedField = 0;
		for(int j=0; j<board->fieldCount; ++j)
		{
			Field* field = &board->fields[j];
			if(field->type != PROPERTY || field->ownerId != board->currentPlayer) continue;

			if(!cheapestOwnedField || GetFieldValue(*cheapestOwnedField) > GetFieldValue(*field))
				cheapestOwnedField = field;
		}
		if(!cheapestOwnedField) break;
	
		int price = GetFieldValue(*cheapestOwnedField);
		player->money += price;
		cheapestOwnedField->ownerId = -1;
		cheapestOwnedField->buildingLevel = 0;
		
		count++;
		sum_cost += price;
		
	}

	if(player->money < 0) AddPopupBoard(board, "Player has bankrupted");
	if(count > 0)
	{
		char buffer[MAX_POPUP_LENGTH] = {0};
		snprintf(buffer, MAX_POPUP_LENGTH, "%i field(s) sold for $%i to pay debt", count, sum_cost);
		AddPopupBoard(board, buffer);
		PlaySound(sounds[KA_CHING_SOUND]);
	}
	
	Timer_Set(&board->delayTimer, 1);
	board->phase = END_ROUND;
}

void HandleEndRound(Board* board)
{
	board->currentPlayer++;
	board->currentPlayer %= board->playerCount;

	int nonBankrupts = 0;
	for(int i=0; i<board->playerCount; ++i)
	{
		if(board->players[i].money >= 0)
			nonBankrupts++;
	}

	if(Timer_HasEnded(board->gameTimer) || nonBankrupts < 2)
	{
		for(int i=0; i<board->fieldCount; ++i)
		{
			Field* field = &board->fields[i];
			if(field->type == PROPERTY && field->ownerId != -1)
			{
				board->players[field->ownerId].money += GetFieldValue(*field);
				field->ownerId = -1;
			}
		}
		for(int i=0; i<board->playerCount; ++i)
		{
			for(int j=i; j<board->playerCount; ++j)
			{
				if(board->players[i].money < board->players[j].money)
				{
					Player player = board->players[i];
					board->players[i] = board->players[j];
					board->players[j] = player;
				}
			}
		}
		board->phase = END_GAME;
		Timer_Set(&board->delayTimer, 3);
	}
	else board->phase = START_ROUND;
	PlaySound(sounds[DING_SOUND]);
}

void HandleEndGame(Board* board)
{
	PlaySound(sounds[DING_SOUND]);
	board->hasGameEnded = true;
}
