#include <board.h>

Board LoadBoard(int size)
{
	if(size > MAX_FIELD_AMOUNT)
		size = MAX_FIELD_AMOUNT;

	else if(size < MIN_FIELD_AMOUNT)
		size = MIN_FIELD_AMOUNT;

	else size &= 0xFFFC;


	Board board = {
		.assets = LoadBoardAssets(),
		.layout = GetBoardLayout(),

		.fieldCount = size,
		.playerCount = 4,
		.players = {
			GetPlayer("Player A", RED),
			GetPlayer("Player B", YELLOW),
			GetPlayer("Player C", GREEN),
			GetPlayer("Player D", BLUE),
		},
	};

	//LoadFields(&board);
	return board;
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

/*

*/
