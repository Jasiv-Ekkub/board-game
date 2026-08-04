#include <board_logic.h>
#include <layout_engine.h>

void UpdateBoardLogic(Board* board, GameContext gameContext)
{
	DrawRectangleRec(GetRectanglePlacement(10, -10, 200, 200, LEFT, BOTTOM, gameContext), BLACK);
	
	DrawTextEx(board->assets.font, boardPhaseNames[board->phase], GetVector2Placement(-500, 10, RIGHT, TOP, gameContext), 36, 1, BLACK);

	switch(board->phase)
	{
		case START_ROUND:
			SetTimer(&board->timer, 1);
			if(UpdateTimer(&board->timer, gameContext))
			{
				board->phase = ROLL_DICE;
			}
			break;

		case ROLL_DICE:
			SetTimer(&board->timer, 5);
			if(UpdateTimer(&board->timer, gameContext))
			{
				board->phase = END_ROUND;
			}
			break;

		case END_ROUND:
			SetTimer(&board->timer, 1);
			if(UpdateTimer(&board->timer, gameContext))
			{
				board->phase = START_ROUND;
			}
			break;
	}
}
