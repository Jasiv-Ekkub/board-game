#include <board_logic.h>
#include <player_logic.h>
#include <layout_engine.h>
#include <animator.h>
#include <game_context.h>
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
void HandleCheckWin(Board* board);
void HandleEndRound(Board* board);
void HandleShowWinner(Board* board);
void HandleEndGame(Board* board);

void UpdateBoardLogic(Board* board)
{	
	board->diceAngle += 90 * GetDeltaTime();
	if(board->phase != END_GAME)
	{
		if(GuiButtonSfx(GetRectanglePlacement(-10, -10, 140, 80, RIGHT, BOTTOM), "Exit"))
			board->forceEnd = true;

		if(board->forceEnd && board->phase < CHECK_WIN)
		{
			board->phase = CHECK_WIN; 
			board->currentDiceroll = 0;
			board->popupCount = 0;
		}

		GuiPlayerInfo(GetRectanglePlacement(0, 10, 225, 120, CENTER, TOP), board->players[board->currentPlayer]);

		GuiPlayerInfo(GetRectanglePlacement(  10, 10, 220, 120, LEFT, TOP), board->players[0]);
		GuiPlayerInfo(GetRectanglePlacement( 240, 10, 220, 120, LEFT, TOP), board->players[1]);

		GuiPlayerInfo(GetRectanglePlacement(-240, 10, 220, 120, RIGHT, TOP), board->players[2]);
		GuiPlayerInfo(GetRectanglePlacement( -10, 10, 220, 120, RIGHT, TOP), board->players[3]);
	}
	else
	{
		if(board->winnerId > -1)
			GuiGameOver(GetRectanglePlacement(0, 0, 500, 200, CENTER, CENTER), board->players[board->winnerId]);
	}

	if(board->popupCount > 0) return; 
	
	Timer_Update(&board->delayTimer);
	if(!Timer_HasEnded(board->delayTimer)) return;

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
		case CHECK_WIN:
			HandleCheckWin(board);
			break;
		case END_ROUND:
			HandleEndRound(board);
			break;
		case SHOW_WINNER:
			HandleShowWinner(board);
			break;
		case END_GAME:
			HandleEndGame(board);
			break;
	}
}

#define CAMERA_TARGET_FACTOR 0.5

void SetSpecialCamera3DTarget(Board* board)
{
	Vector3 cameraTarget = board->position;

	int quarter = board->fieldCount/4;
	Player player = board->players[board->currentPlayer];
	
	int side = player.position / quarter;
	bool isCorner = player.position % quarter == 0;

	switch(side)
	{
		case 0:
		cameraTarget.x -= CAMERA_TARGET_FACTOR;
		break;

		case 1:
		cameraTarget.z -= CAMERA_TARGET_FACTOR;
		break;
		
		case 2:
		cameraTarget.x += CAMERA_TARGET_FACTOR;
		break;
		
		case 3:
		cameraTarget.z += CAMERA_TARGET_FACTOR;
		break;
		
		default:
		break;
	}
	if(isCorner)
	{
		switch(side)
		{
			case 0:
			cameraTarget.z += CAMERA_TARGET_FACTOR;
			break;
			
			case 1:
			cameraTarget.x -= CAMERA_TARGET_FACTOR;
			break;
			
			case 2:
			cameraTarget.z -= CAMERA_TARGET_FACTOR;
			break;
			
			case 3:
			cameraTarget.x += CAMERA_TARGET_FACTOR;
			break;

			default:
			break;
		}
	}

	SetCamera3DTarget(cameraTarget, 1);
}

void HandleStartRound(Board* board)
{
	SetCamera3DTarget(board->position, 1);

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
	Vector3 playerModelPosition = CalculatePlayerPosition(*board, board->currentPlayer, player->position);
	AddKeyframe(&animation, 1, playerModelPosition);
	QueueAnimation(animation);
	SetSpecialCamera3DTarget(board);
	
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
			board->phase = CHECK_FIELD;
		}
		else board->phase = CHECK_DEBT;
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
			board->phase = CHECK_FIELD;
		}
		else
		{
			board->phase = CHECK_DEBT;
		}
	}
}

void HandleCheckDebt(Board* board)
{
	Player* player = &board->players[board->currentPlayer];

	int count = 0;
	int sum_cost = 0;

	while(player->money < 0)
	{
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
	board->phase = CHECK_WIN;
}

void SellAllFields(Board* board)
{
	bool playSound = false;
	for(int i=0; i<board->fieldCount; ++i)
	{
		Field* field = &board->fields[i];
		if(field->type == PROPERTY && field->ownerId != -1)
		{
			board->players[field->ownerId].money += GetFieldValue(*field);
			field->ownerId = -1;
			field->buildingLevel = 0;
			playSound = true;
		}
	}
	if(playSound) PlaySound(sounds[KA_CHING_SOUND]);
}

void HandleCheckWin(Board* board)
{

	//Bankrupts
	board->phase = SHOW_WINNER;
	for(int i=0; i<board->playerCount; ++i)
	{
		Player player = board->players[i];
		if(player.money < 0) continue;

		if(board->winnerId == -1)
		{
			board->winnerId = i;
		}
		else
		{
			board->winnerId = -1;
			break;
		}
	}
	if(board->winnerId != -1)
	{
		SellAllFields(board);
		AddPopupBoard(board, "There is only one active player left");
		return;
	}

	//Monopoly
	int monopolyGroupOwners[8] = { -2, -2, -2, -2, -2, -2, -2, -2 };
	for(int i=0; i<board->fieldCount; ++i)
	{
		Field field = board->fields[i];
		int groupId = field.groupId;
		if(field.type == PROPERTY)
		{
			if(monopolyGroupOwners[groupId] == -2)
			{
				monopolyGroupOwners[groupId] = field.ownerId;
			}
			else if(monopolyGroupOwners[groupId] != field.ownerId)
			{
				monopolyGroupOwners[groupId] = -1;
			}
		}
	}
	int playerMonopolyCounters[4] = {0};
	for(int i=0; i<8; ++i) 
	{
		int monopolyGroupOwner = monopolyGroupOwners[i];
		if(monopolyGroupOwner < 0 || monopolyGroupOwner > 3) continue;
		if(++playerMonopolyCounters[monopolyGroupOwner] >= 3)
		{
			board->winnerId = monopolyGroupOwner;
			SellAllFields(board);
			AddPopupBoard(board, "Player has achieved triple monopoly");
			return;
		}
	}

	//Exit
	if(board->forceEnd)
	{
		SellAllFields(board);

		board->winnerId = 0;
		for(int i=1; i<board->playerCount; ++i)
		{
			if(board->players[board->winnerId].money < board->players[i].money)
				board->winnerId = i;
		}
		AddPopupBoard(board, "Time is up, the wealthiest wins");
		return;
	}

	//Continue
	if(board->winnerId == -1)
		board->phase = END_ROUND;
}

void HandleEndRound(Board* board)
{
	board->currentPlayer++;
	board->currentPlayer %= board->playerCount;
	board->phase = START_ROUND;
	Timer_Set(&board->delayTimer, 0.5);
	PlaySound(sounds[DING_SOUND]);
}

void HandleShowWinner(Board* board)
{
	SetCamera3DTarget(board->position, 1);
	board->phase = END_GAME;
	Timer_Set(&board->delayTimer, 3);
	PlaySound(sounds[DING_SOUND]);
}

void HandleEndGame(Board* board)
{
	PlaySound(sounds[DING_SOUND]);
	board->hasGameEnded = true;
}
