#include <board.h>
#include <board_setup.h>
#include <board_logic.h>

#include <layout_engine.h>
#include <gui_elements.h>
#include <game_context.h>
#include <assets.h>
#include <raygui.h>

#include <stdio.h>

const char* boardPhaseNames[] = {
	[START_ROUND] = "Round beginning",
	[ROLL_DICE] = "Rolling dice",
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

Board LoadBoard()
{
	return (Board){
		.layout = GetBoardLayout(),
		.timer = GetTimer(TIMER_SINGLE_PULSE),
		.gameTimer = GetTimer(TIMER_SINGLE_PULSE | TIMER_INTERRUPTABLE),
		.popupTimer = GetTimer(TIMER_SINGLE_PULSE | TIMER_REPEATING),

		.colors = {
			.light = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)),
			.dark = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)),
		}
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
			CalculatePlayerPosition(board, i),
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

void UpdateBoard(Board* board)
{
	if(board->hasGameEnded) return;

	BeginMode3D(GetCamera3D());
		DrawModel(models[BOARD_MODEL], board->position, 1, WHITE);
		DrawPlayers(*board);
		DrawBuildings(*board);
	EndMode3D();

	UpdateBoardLogic(board);

	if(board->popupCount > 0)
	{
		GuiBoxText(GetRectanglePlacement(0,0,600,150,CENTER,CENTER), board->popups[board->popupCount-1]);
		if(UpdateTimer(&board->popupTimer))
		{
			board->popupCount--;
		}
	}
}

void UnloadBoard(Board board)
{
}

bool HasGameEndedBoard(Board board) { return board.hasGameEnded; }

void AddPopupBoard(Board* board, const char* popup)
{
	if(board->popupCount < MAX_POPUP_COUNT)
	{
		board->popups[board->popupCount++] = popup;
	}
}
