#include <board.h>
#include <board_setup.h>
#include <board_logic.h>

Board LoadBoard()
{
	Board board = {
		.assets = LoadBoardAssets(),
		.layout = GetBoardLayout(),

		.playerCount = 4,
		.players = {
			GetPlayer("Player A", RED),
			GetPlayer("Player B", YELLOW),
			GetPlayer("Player C", GREEN),
			GetPlayer("Player D", BLUE),
		},

		.timer = GetTimer(1, 0),
	};

	return board;
}

void SetBoardSize(Board* board, int size)
{
	if(size > MAX_FIELD_AMOUNT)
		size = MAX_FIELD_AMOUNT;
	else if(size < MIN_FIELD_AMOUNT)
		size = MIN_FIELD_AMOUNT;
	else size &= 0xFFFC;

	board->fieldCount = size;
	LoadFields(board);
	GenerateBoardTexture(board);

	for(int i=0; i<board->fieldCount; ++i)
	{
		PrintField(board->fields[i]);
	}
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
		if(field.type != PROPERTY || field.buildingLevel == 0 || field.ownerId == -1) continue;
		
		BoardModelId modelId = board.assets.buildingLevelModelId[field.buildingLevel - 1];
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

	UpdateBoardLogic(board, gameContext);
}

void UnloadBoard(Board board)
{
	UnloadBoardAssets(board.assets);
}
