#include <board.h>
#include <board_setup.h>
#include <board_logic.h>

#include <layout_engine.h>
#include <gui_elements.h>
#include <game_context.h>
#include <assets.h>
#include <raygui.h>

#include <stdio.h>
#include <string.h>

const char* boardPhaseNames[] = {
	[START_ROUND] = "Round beginning",
	[ROLL_DICE] = "Rolling dice",
	[MOVE_PLAYER] = "Moving player",
	[CHECK_FIELD] = "Checking field",
	[PAY_FEE] = "Paying fee",
	[BUY_FIELD] = "Buying field",
	[UPGRADE_BUILDING] = "Upgrading building",
	[CHECK_DEBT] = "Paying debt",
	[END_ROUND] = "Round ending",
};

static const ModelId buildingLevelModelId[4] = {
	SITE_MODEL,
	HOUSE_MODEL,
	VILLA_MODEL,
	APARTAMENT_MODEL,
};

Board GetBoard()
{
	return (Board){
		.layout = GetBoardLayout(),
		.delayTimer = Timer_Get(),
		.popupTimer = Timer_Get(),

		.colors = {
			.light = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)),
			.dark = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)),
		},

		.diceAngle = 0,
		.diceAxis = {1, 2, 4},
	};
}

void SetupBoard(Board* board, int size, int humanCount, int botCount)
{
	if(size > MAX_FIELD_COUNT)
		size = MAX_FIELD_COUNT;
	else if(size < MIN_FIELD_COUNT)
		size = MIN_FIELD_COUNT;
	else size &= 0xFFFC;

	board->fieldCount = size;
	LoadFields(board);
	LoadPlayers(board, 3000, humanCount, botCount);
	GenerateBoardTexture(board);
	
	for(int i=0; i<board->playerCount; ++i)
	{
		board->players[i].modelPosition = CalculatePlayerPosition(*board, i, 0);
	}

	Timer_Set(&board->popupTimer, 3);
	board->phase = START_ROUND;
	board->forceEnd = false;
	board->hasGameEnded = false;
}

void DrawPlayers(Board board)
{
	for(int i=0; i<board.playerCount; ++i)
	{
		DrawModel(
			models[PAWN_MODEL],
			board.players[i].modelPosition,
			board.layout.modelScale,
			board.players[i].color);
	}
}

void DrawBuildings(Board board)
{
	for(int i=0; i<board.fieldCount; ++i)
	{
		Field field = board.fields[i];
		if(field.type != PROPERTY || field.ownerId == -1) continue;
		
		ModelId modelId = buildingLevelModelId[field.buildingLevel];
		Color ownerColor = board.players[field.ownerId].color;

		DrawModel(
			models[modelId],
			CalculateHousePosition(board, i),
			board.layout.modelScale,
			ownerColor);
	}
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

void DrawDice(Board board)
{
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
}

void UpdateBoard(Board* board)
{
	if(board->hasGameEnded) return;

	BeginMode3D(GetCamera3D());
		DrawModel(models[BOARD_MODEL], board->position, 1, WHITE);
		DrawPlayers(*board);
		DrawBuildings(*board);
		DrawDice(*board);
	EndMode3D();

	UpdateBoardLogic(board);

	if(board->popupCount > 0)
	{
		if(Timer_HasEnded(board->popupTimer))
		{
			PlaySound(sounds[POPUP_SOUND]);
			Timer_Set(&board->popupTimer, 2);
		}
		GuiBoxText(GetRectanglePlacement(0, -50, 600, 150, CENTER, BOTTOM), board->popups[board->popupCount-1]);
		Timer_Update(&board->popupTimer);

		if(Timer_HasEnded(board->popupTimer))
		{
			board->popupCount--;
		}
	}
}

bool HasGameEndedBoard(Board board) { return board.hasGameEnded; }

void AddPopupBoard(Board* board, const char* popup)
{
	if(board->popupCount < MAX_POPUP_COUNT)
	{
		strncpy(board->popups[board->popupCount++], popup, MAX_POPUP_LENGTH);
	}
}
