#include <board.h>
#include <board_setup.h>
#include <board_logic.h>

#include <layout_engine.h>
#include <gui_elements.h>
#include <game_context.h>

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

Board LoadBoard()
{
	return (Board){
		.assets = LoadBoardAssets(),
		.layout = GetBoardLayout(),
		.timer = GetTimer(TIMER_SINGLE_PULSE),
		.gameTimer = GetTimer(TIMER_SINGLE_PULSE),
		.popupTimer = GetTimer(TIMER_SINGLE_PULSE | TIMER_REPEATING | TIMER_INTERRUPTABLE),
	};
}

void SetupBoard(Board* board, int size, int humanCount, int botCount)
{
	if(size > MAX_FIELD_AMOUNT)
		size = MAX_FIELD_AMOUNT;
	else if(size < MIN_FIELD_AMOUNT)
		size = MIN_FIELD_AMOUNT;
	else size &= 0xFFFC;

	board->fieldCount = size;
	LoadFields(board);
	LoadPlayers(board, 1000, humanCount, botCount);
	GenerateBoardTexture(board);
	
	//SetTimer(&board->gameTimer, 1800);
	SetTimer(&board->gameTimer, 1);
	SetTimer(&board->popupTimer, 3);
	board->phase = START_ROUND;
}

void DrawPlayers(Board board)
{
	for(int i=0; i<board.playerCount; ++i)
	{
		DrawModel(
			board.assets.models[PAWN_MODEL],
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
		
		BoardModelId modelId = board.assets.buildingLevelModelId[field.buildingLevel];
		Color ownerColor = board.players[field.ownerId].color;

		DrawModel(
			board.assets.models[modelId],
			CalculateHousePosition(board, i),
			board.layout.modelScale,
			ownerColor);
	}
}

void UpdateBoard(Board* board)
{
	if(HasGameEndedBoard(*board)) return;

	BeginMode3D(GetCamera3D());
		DrawModel(board->assets.models[BOARD_MODEL], board->position, 1, WHITE);
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
	UnloadBoardAssets(board.assets);
}

bool HasGameEndedBoard(Board board) { return board.hasGameEnded; }

void AddPopupBoard(Board* board, const char* popup)
{
	if(board->popupCount < MAX_POPUP_AMOUNT)
	{
		board->popups[board->popupCount++] = popup;
	}
}
