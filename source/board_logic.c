#include <board_logic.h>
#include <stdio.h>


void UpdateBoardLogic(Board* board, GameContext gameContext)
{
	if(UpdateTimer(&board->timer, gameContext))
	{
		printf("Update 1 sec\n");
		board->players[0].position += 1;
		board->players[0].position %= board->fieldCount;
	}
}
