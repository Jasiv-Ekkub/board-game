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
	[END_GAME] = "Game over",
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
		.timer = GetTimer(TIMER_SINGLE_PULSE),
		.gameTimer = GetTimer(TIMER_SINGLE_PULSE | TIMER_INTERRUPTABLE),
		.popupTimer = GetTimer(TIMER_SINGLE_PULSE | TIMER_REPEATING),

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
	LoadPlayers(board, 1000, humanCount, botCount);
	GenerateBoardTexture(board);
	
	for(int i=0; i<board->playerCount; ++i)
	{
		board->playerModelPositions[i] = CalculatePlayerPosition(*board, i, 0);
	}

	SetTimer(&board->gameTimer, 1800);
	SetTimer(&board->popupTimer, 3);
	board->phase = START_ROUND;
	board->hasGameEnded = false;
}

void DrawPlayers(Board board)
{
	for(int i=0; i<board.playerCount; ++i)
	{
		DrawModel(
			models[PAWN_MODEL],
			board.playerModelPositions[i],
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
	const int roll = board.currentDiceroll;
	if(roll > 0 && roll < 7)
	{
		DrawModelEx(
			models[DICE_MODEL],
			(Vector3){0},
			diceTransforms[roll-1].axis,
			diceTransforms[roll-1].angle,
			(Vector3){1,1,1}, WHITE
		);
	}
	else if(roll == -1)
	{
		DrawModelEx(
			models[DICE_MODEL],
			(Vector3){0},
			board.diceAxis,
			board.diceAngle,
			(Vector3){1,1,1}, WHITE
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
	EndMode3D();


	BeginMode3D(GetCameraDice());
		DrawDice(*board);
	EndMode3D();

	UpdateBoardLogic(board);

	if(board->popupCount > 0)
	{
		if(!board->playedPopupSound)
		{
			PlaySound(sounds[POPUP_SOUND]);
			board->playedPopupSound = true;
		}
		GuiBoxText(GetRectanglePlacement(0,0,600,150,CENTER,CENTER), board->popups[board->popupCount-1]);
		if(UpdateTimer(&board->popupTimer))
		{
			board->popupCount--;
			board->playedPopupSound = false;
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
