#include <board_logic.h>
#include <layout_engine.h>

void UpdateBoardLogic(Board* board, GameContext gameContext)
{
	DrawRectangleRec(GetRectanglePlacement(0, 0, 200, 200, CENTER, CENTER, gameContext), BLACK);
}
