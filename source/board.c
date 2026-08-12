#include <board.h>
#include <board_setup.h>
#include <board_logic.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const char* boardPhaseNames[] = {
	[START_ROUND] = "Round beginning",
	[ROLL_DICE] = "Rolling dice",
	[CHECK_FIELD] = "Checking field",
	[PAY_FEE] = "Paying fee",
	[BUY_FIELD] = "Buying field",
	[UPGRADE_BUILDING] = "Upgrading building",
	[CHECK_DEBT] = "Paying debt",
	[END_ROUND] = "Round ending",
};

Board LoadBoard()
{
	srand(time(0));
	return (Board){
		.assets = LoadBoardAssets(),
		.layout = GetBoardLayout(),
		.timer = GetTimer(TIMER_SINGLE_PULSE),
		.gameTimer = GetTimer(TIMER_SINGLE_PULSE),
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
	
	SetTimer(&board->gameTimer, 1800);
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

void UpdateBoard(Board* board, GameContext gameContext)
{
	BeginMode3D(gameContext.camera3D);
		DrawModel(board->assets.models[BOARD_MODEL], board->position, 1, WHITE);
		DrawPlayers(*board);
		DrawBuildings(*board);
	EndMode3D();

	if(!HasGameEnded(*board)) UpdateBoardLogic(board, gameContext);
}

void UnloadBoard(Board board)
{
	UnloadBoardAssets(board.assets);
}

bool HasGameEnded(Board board)
{
	if(HasTimerEnded(board.gameTimer)) return true;

	if(board.phase != START_ROUND) return false;

	int activePlayers = 0;
	for(int i=0; i<board.playerCount; ++i)
	{
		if(board.players[i].money >= 0)
		{
			activePlayers++;
		}
	}
	return activePlayers < 2;
}

Player GetWinner(Board board)
{
	int wealth[MAX_PLAYER_AMOUNT] = {
		board.players[0].money,
		board.players[1].money,
		board.players[2].money,
		board.players[3].money,
	};
	for(int i=0; i<board.fieldCount; ++i)
	{
		Field *field = &board.fields[i];
		if(field->type == PROPERTY && field->ownerId != -1)
		{
			wealth[field->ownerId] += GetFieldValue(*field);
		}
	}

	int winnerId = 0;
	for(int i=1; i<board.playerCount; ++i)
	{
		if(wealth[winnerId] < wealth[i]) winnerId = i; 
	}
	board.players[winnerId].money = wealth[winnerId];
	return board.players[winnerId];
}
