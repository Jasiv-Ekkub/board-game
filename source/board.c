#include <board.h>
#include <board_setup.h>

#include <layout_engine.h>
#include <gui_elements.h>
#include <game_context.h>
#include <assets.h>
#include <raygui.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <player_logic.h>
#include <animator.h>
#include <helpers.h>

Board board;

static const ModelId buildingLevelModelId[4] = {
	SITE_MODEL,
	HOUSE_MODEL,
	VILLA_MODEL,
	APARTAMENT_MODEL,
};

void InitializeBoard()
{
	board.layout = GetBoardLayout();
	board.delayTimer = Timer_Get();
	board.popupTimer = Timer_Get();

	board.colors.light = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)),
	board.colors.dark = GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)),
	board.colors.text = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)),

	board.diceAngle = 0;
	board.diceAxis = (Vector3){0};
}

void SetupBoard(int size, int humanCount, int botCount, int startMoney)
{
	if(size > MAX_FIELD_COUNT)
		size = MAX_FIELD_COUNT;
	else if(size < MIN_FIELD_COUNT)
		size = MIN_FIELD_COUNT;
	else size &= 0xFFFC;

	board.fieldCount = size;
	LoadFields();
	LoadPlayers(startMoney, humanCount, botCount);
	GenerateBoardTexture();
	
	for(int i=0; i<board.playerCount; ++i)
	{
		board.players[i].modelPosition = CalculatePlayerPosition(i, 0);
	}

	Timer_Set(&board.popupTimer, 0);
	board.phase = START_ROUND;
	board.forceEnd = false;
	board.hasGameEnded = false;
}

static const struct {
	Vector3 axis;
	float angle;
} diceTransforms[6] = {
	{{0, 0, 0}, 0},
	{{0, 0, 1}, 90},
	{{1, 0, 0}, 90},
	{{1, 0, 0}, -90},
	{{0, 0, 1}, -90},
	{{1, 0, 0}, 180},
};

void UpdateBoardLogic();

void UpdateBoard()
{
	if(board.hasGameEnded) return;

	BeginMode3D(GetCamera3D());
		DrawModel(models[BOARD_MODEL], board.position, 1, WHITE);
		for(int i=0; i<board.playerCount; ++i)
		{
			DrawModel(
				models[PAWN_MODEL],
				board.players[i].modelPosition,
				board.layout.modelScale,
				board.players[i].color);
		}
		for(int i=0; i<board.fieldCount; ++i)
		{
			Field field = board.fields[i];
			if(field.type != PROPERTY || field.ownerId == -1) continue;
			
			ModelId modelId = buildingLevelModelId[field.buildingLevel];
			Color ownerColor = board.players[field.ownerId].color;

			DrawModel(
				models[modelId],
				CalculateHousePosition(i),
				board.layout.modelScale,
				ownerColor);
		}
		Vector3 position = board.position;
		position.x -= 0.3f;
		position.y += 0.5f;
		position.z += 0.3f;

		const int roll = board.currentDiceroll;
		if(roll > 0 && roll < 7)
		{
			DrawModelEx(
				models[DICE_MODEL],
				position,
				diceTransforms[roll-1].axis,
				diceTransforms[roll-1].angle,
				(Vector3){ 0.2, 0.2, 0.2 }, WHITE
			);
		}
		else if(roll == -1)
		{
			DrawModelEx(
				models[DICE_MODEL],
				position,
				board.diceAxis,
				board.diceAngle,
				(Vector3){ 0.2, 0.2, 0.2 }, WHITE
			);
		}
	EndMode3D();

	UpdateBoardLogic();

	if(board.popupCount > 0)
	{
		if(Timer_HasEnded(board.popupTimer))
		{
			PlaySound(sounds[POPUP_SOUND]);
			Timer_Set(&board.popupTimer, 2);
		}
		GuiBoxText(GetRectanglePlacement(0, -50, 600, 200, CENTER, BOTTOM), board.popups[board.popupCount-1]);
		Timer_Update(&board.popupTimer);

		if(Timer_HasEnded(board.popupTimer))
		{
			board.popupCount--;
		}
	}
}

bool HasGameEndedBoard() { return board.hasGameEnded; }

void AddPopupBoard(const char* popup)
{
	if(board.popupCount < MAX_POPUP_COUNT)
	{
		strncpy(board.popups[board.popupCount++], popup, MAX_POPUP_LENGTH);
	}
}

#define BUFFER_SIZE 64

void HandleStartRound();
void HandleRollDice();
void HandleMovePlayer();
void HandleCheckField();
void HandlePayFee();
void HandleBuyField();
void HandleUpgradeBuilding();
void HandleCheckDebt();
void HandleCheckWin();
void HandleEndRound();

void UpdateBoardLogic()
{	
	board.diceAngle += 90 * GetDeltaTime();
	if(board.phase != END_GAME)
	{
		if(GuiButtonSfx(GetRectanglePlacement(-10, -10, 140, 80, RIGHT, BOTTOM), "Exit"))
			board.forceEnd = true;

		if(board.forceEnd && board.phase < CHECK_WIN)
		{
			board.phase = CHECK_WIN; 
			board.currentDiceroll = 0;
			board.popupCount = 0;
			Timer_Set(&board.delayTimer, 0.5);
		}

		GuiPlayerInfo(GetRectanglePlacement(0, 10, 225, 120, CENTER, TOP), board.players[board.currentPlayer]);

		GuiPlayerInfo(GetRectanglePlacement(  10, 10, 220, 120, LEFT, TOP), board.players[0]);
		GuiPlayerInfo(GetRectanglePlacement( 240, 10, 220, 120, LEFT, TOP), board.players[1]);

		GuiPlayerInfo(GetRectanglePlacement(-240, 10, 220, 120, RIGHT, TOP), board.players[2]);
		GuiPlayerInfo(GetRectanglePlacement( -10, 10, 220, 120, RIGHT, TOP), board.players[3]);
	}
	else
	{
		if(board.winnerId > -1)
			GuiGameOver(GetRectanglePlacement(0, 0, 500, 200, CENTER, CENTER), board.players[board.winnerId]);
	}

	if(board.popupCount > 0) return; 
	
	Timer_Update(&board.delayTimer);
	if(!Timer_HasEnded(board.delayTimer)) return;

	switch(board.phase)
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
			board.currentPlayer++;
			board.currentPlayer %= board.playerCount;
			board.phase = START_ROUND;
			Timer_Set(&board.delayTimer, 0.5);
			PlaySound(sounds[DING_SOUND]);
			break;
		case SHOW_WINNER:
			SetCamera3DTarget(board.position, 1);
			board.phase = END_GAME;
			Timer_Set(&board.delayTimer, 3);
			PlaySound(sounds[DING_SOUND]);
			break;
		case END_GAME:
			PlaySound(sounds[DING_SOUND]);
			board.hasGameEnded = true;
			break;
	}
}

#define CAMERA_TARGET_FACTOR 0.5

void SetSpecialCamera3DTarget()
{
	Vector3 cameraTarget = board.position;

	int quarter = board.fieldCount/4;
	Player player = board.players[board.currentPlayer];
	
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

void HandleStartRound()
{
	SetCamera3DTarget(board.position, 1);

	Player* player = &board.players[board.currentPlayer];
	bool skip = false;
	if(player->turnSkips > 0)
	{
		char buffer[MAX_POPUP_LENGTH] = {0};
		snprintf(buffer, MAX_POPUP_LENGTH, "Player skips %i round(s)", player->turnSkips--);
		AddPopupBoard(buffer);
		skip = true;
	}

	skip |= player->money < 0;
	if(skip)
	{
		board.phase = END_ROUND;
		return;
	}
	board.diceAxis = (Vector3){
		GetRandomFloat(-1, 1),
		GetRandomFloat(-1, 1),
		GetRandomFloat(-1, 1)
	};
	board.currentDiceroll = -1;
	board.phase = ROLL_DICE;
	Timer_Set(&board.delayTimer, 0.5);
}

void HandleRollDice()
{
	if(GetPlayerResponse(board))
	{
		board.currentDiceroll = 1 + rand()%6;
		
		board.phase = MOVE_PLAYER;
		PlaySound(sounds[DICE_HIT_SOUND]);
		Timer_Set(&board.delayTimer, 1);
	}
}

void HandleMovePlayer()
{
	Player* player = &board.players[board.currentPlayer];
	for(int i=1; i<=board.currentDiceroll; ++i)
	{
		int position = (player->position + i) % board.fieldCount;
		Field field = board.fields[position];
		if(field.type == SUPERACTION)
		{
			field.action(board);
		}
	}
	int oldPosition = player->position;
	player->position += board.currentDiceroll;
	player->position %= board.fieldCount;

	int quarter = board.fieldCount/4;
	int ppd = player->position/quarter;
	Animation animation = GetAnimation(&board.players[board.currentPlayer].modelPosition);
	if(oldPosition/quarter != ppd)
	{
		AddKeyframe(&animation, 0.5, CalculatePlayerPosition(board.currentPlayer, ppd * quarter));
	}
	Vector3 playerModelPosition = CalculatePlayerPosition(board.currentPlayer, player->position);
	AddKeyframe(&animation, 1, playerModelPosition);
	QueueAnimation(animation);
	SetSpecialCamera3DTarget(board);
	
	board.currentDiceroll = 0;
	board.phase = CHECK_FIELD;
}

void HandleCheckField()
{
	Player* player = &board.players[board.currentPlayer];
	
	Field* field = &board.fields[player->position];
	board.phase = CHECK_DEBT;
	switch(field->type)
	{
		case PROPERTY:
			if(field->ownerId == -1)
			{
				if(GetFieldValue(*field) <= player->money)
				{
					board.phase = BUY_FIELD;
					return;
				}
			}
			else if(field->ownerId == board.currentPlayer)
			{
				if(field->buildingLevel < MAX_BUILDING_LEVEL - 1 && GetUpgradeValue(*field) <= player->money)
				{
					board.phase = UPGRADE_BUILDING;	
					return;
				}
			}
			else
			{
				board.phase = PAY_FEE;
				return;
			}
			break;
		
		case ACTION:
			field->action(board);
			break;
		default:
			break;
	}
}

void HandlePayFee()
{
	Player* player = &board.players[board.currentPlayer];
	Field* field = &board.fields[player->position];
	Player* owner = &board.players[field->ownerId];

	int fee = GetFeeValue(*field);

	char buffer[MAX_POPUP_LENGTH] = {0};
	snprintf(buffer, MAX_POPUP_LENGTH, "The amount due is $%i", fee);
	AddPopupBoard(buffer);
	player->money -= fee;
	owner->money += fee;

	if(GetFieldValue(*field) <= player->money) board.phase = BUY_FIELD;
	else board.phase = CHECK_DEBT;

	PlaySound(sounds[KA_CHING_SOUND]);
}

void HandleBuyField()
{
	if(GetPlayerResponse(board))
	{
		if(board.currentPlayerResponse == POSITIVE)
		{
			Player* player = &board.players[board.currentPlayer];
			Field* field = &board.fields[player->position];
			
			int cost = GetFieldValue(*field);
			char buffer[MAX_POPUP_LENGTH] = {0};
			snprintf(buffer, MAX_POPUP_LENGTH, "%s was bought\nfor $%i", field->name, cost);
			AddPopupBoard(buffer);

			if(field->ownerId != -1)
			{
				Player* owner = &board.players[field->ownerId];
				owner->money += cost;
			}
			player->money -= cost;
			field->ownerId = board.currentPlayer;
			PlaySound(sounds[KA_CHING_SOUND]);
			board.phase = CHECK_FIELD;
		}
		else board.phase = CHECK_DEBT;
	}
}

void HandleUpgradeBuilding()
{
	if(GetPlayerResponse(board))
	{
		if(board.currentPlayerResponse == POSITIVE)
		{
			Player* player = &board.players[board.currentPlayer];
			Field* field = &board.fields[player->position];

			int cost = GetUpgradeValue(*field);
			char buffer[MAX_POPUP_LENGTH] = {0};
			snprintf(buffer, MAX_POPUP_LENGTH, "%s was developed\nfor $%i", field->name, cost);
			AddPopupBoard(buffer);
			
			player->money -= cost;
			field->buildingLevel++;
			PlaySound(sounds[KA_CHING_SOUND]);
			board.phase = CHECK_FIELD;
		}
		else
		{
			board.phase = CHECK_DEBT;
		}
	}
}

void HandleCheckDebt()
{
	Player* player = &board.players[board.currentPlayer];

	int count = 0;
	int sum_cost = 0;

	while(player->money < 0)
	{
		Field* cheapestOwnedField = 0;
		for(int j=0; j<board.fieldCount; ++j)
		{
			Field* field = &board.fields[j];
			if(field->type != PROPERTY || field->ownerId != board.currentPlayer) continue;

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

	if(player->money < 0) AddPopupBoard("Player has bankrupted");
	if(count > 0)
	{
		char buffer[MAX_POPUP_LENGTH] = {0};
		snprintf(buffer, MAX_POPUP_LENGTH, "%i field(s) sold for\n$%i to pay debt", count, sum_cost);
		AddPopupBoard(buffer);
		PlaySound(sounds[KA_CHING_SOUND]);
	}
	
	Timer_Set(&board.delayTimer, 1);
	board.phase = CHECK_WIN;
}

void SellAllFields()
{
	bool playSound = false;
	for(int i=0; i<board.fieldCount; ++i)
	{
		Field* field = &board.fields[i];
		if(field->type == PROPERTY && field->ownerId != -1)
		{
			board.players[field->ownerId].money += GetFieldValue(*field);
			field->ownerId = -1;
			field->buildingLevel = 0;
			playSound = true;
		}
	}
	if(playSound) PlaySound(sounds[KA_CHING_SOUND]);
}

void HandleCheckWin()
{

	//Bankrupts
	board.phase = SHOW_WINNER;
	for(int i=0; i<board.playerCount; ++i)
	{
		Player player = board.players[i];
		if(player.money < 0) continue;

		if(board.winnerId == -1)
		{
			board.winnerId = i;
		}
		else
		{
			board.winnerId = -1;
			break;
		}
	}
	if(board.winnerId != -1)
	{
		SellAllFields();
		AddPopupBoard("There is only one\nactive player left");
		return;
	}

	//Monopoly
	int monopolyGroupOwners[8] = { -2, -2, -2, -2, -2, -2, -2, -2 };
	for(int i=0; i<board.fieldCount; ++i)
	{
		Field field = board.fields[i];
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
			board.winnerId = monopolyGroupOwner;
			SellAllFields();
			AddPopupBoard("Player has achieved\ntriple monopoly");
			return;
		}
	}

	//Exit
	if(board.forceEnd)
	{
		SellAllFields();

		board.winnerId = 0;
		for(int i=1; i<board.playerCount; ++i)
		{
			if(board.players[board.winnerId].money < board.players[i].money)
				board.winnerId = i;
		}
		AddPopupBoard("Time is up,\n the wealthiest wins");
		return;
	}

	//Continue
	board.phase = END_ROUND;
}
