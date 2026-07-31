#include <board.h>
#include <board_setup.h>

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

void UpdateBoard(Board* board, GameContext gameContext)
{
	DrawModel(board->assets.models[BOARD_MODEL], board->position, 1, WHITE);
	DrawModel(board->assets.models[PAWN_MODEL], board->position, 0.05, RED);
	DrawModel(board->assets.models[HOUSE_MODEL], board->position, 0.05, BLUE);
}

void UnloadBoard(Board board)
{
	UnloadBoardAssets(board.assets);
}
